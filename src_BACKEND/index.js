import Fastify from 'fastify';
import fs from 'fs';Fastify
const fastify = Fastify({ logger: true});

// temp 'database'
const LOG_FILE = './data.log';

// get data from the bridge
fastify.post('/ingest', async(request, reply) => {
    const record = {
	server_time: new Date().toISOString(),
	source: request.headers['x-source'] || 'unknown',
	...request.body
    };

    // holding in a log file for now, will add database later
    fs.appendFileSync(LOG_FILE, JSON.stringify(record) + '\n');

    return { status: 'ok'};
});

// send latest data when requested
fastify.get('/recent', async () => {
    if (!fs.existsSync(LOG_FILE)) return [];

    // getting data from the log file for now
    const lines = fs.readFileSync(LOG_FILE, 'utf-8')
	  .trim()
	  .split('\n')
	  .slice(-1)

    return JSON.parse(lines[0]);
});

fastify.listen({ port: 3000, host: '0.0.0.0' });
