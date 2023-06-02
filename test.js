let binding = require('./index.js');

(async () => {
    let host = await binding.VixHost.ConnectLocal();
    console.log(host)
})()