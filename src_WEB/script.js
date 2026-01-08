async function fetchData() {
    try {
	const res = await fetch('http://127.0.0.1:3000/recent');
	const data = await res.json();

	document.getElementById('data').value = data.payload.data;
    } catch (err) {
	console.error('failed to fetch data', err);
    }
}

fetchData();
setInterval(fetchData, 1000);
