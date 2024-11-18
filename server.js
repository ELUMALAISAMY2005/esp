const express = require('express');
const axios = require('axios');

const app = express();
const ESP32_IP = 'http://192.168.1.15'; // Replace with ESP32's IP

app.use(express.static('public'));

// Start blinking
app.get('/blink', async (req, res) => {
  const count = req.query.count;
  if (!count) {
    return res.status(400).send({ error: 'Missing parameter: count' });
  }
  try {
    const response = await axios.get(`${ESP32_IP}/blink?count=${count}`);
    res.send(response.data);
  } catch (error) {
    res.status(500).send({ error: 'ESP32 communication failed' });
  }
});

// Get blinking status
app.get('/status', async (req, res) => {
  try {
    const response = await axios.get(`${ESP32_IP}/status`);
    res.send(response.data);
  } catch (error) {
    res.status(500).send({ error: 'ESP32 communication failed' });
  }
});

// Start server
app.listen(3007, () => {
  console.log('Server running at http://localhost:3007');
});
