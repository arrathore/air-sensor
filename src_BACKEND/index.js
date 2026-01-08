import Fastify from 'fastify';
import fs from 'fs';Fastify
const fastify = Fastify({ logger: true});

const LOG_FILE = './data.log';

fastify.post('/ingest', async(request, reply) => {
    const record = {
	ts: new Date().toISOString(),
	source: request.headers['x-source'] || 'unknown',
	...request.body
    };

    fs.appendFileSync(LOG_FILE, JSON.stringify(record) + '\n');

    return { status: 'ok'};
});

fastify.get('/recent', async () => {
    if (!fs.existsSync(LOG_FILE)) return [];

    const lines = fs.readFileSync(LOG_FILE, 'utf-8')
	  .trim()
	  .split('\n')
	  .slice(-100)

    return lines.map(JSON.parse);
});

fastify.listen({ port: 3000, host: '0.0.0.0' });
