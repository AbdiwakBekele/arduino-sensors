const express = require('express');
const bodyParser = require('body-parser');
const mysql = require('mysql2');
const path = require('path');
const app = express();
const port = 3000;

// Create a connection to the MySQL database
const db = mysql.createConnection({
  host: 'localhost',
  user: 'yourUsername',
  password: 'yourPassword',
  database: 'soilData'
});

// Connect to MySQL
db.connect((err) => {
  if (err) {
    throw err;
  }
  console.log('Connected to MySQL');

  // Create the table if it doesn't exist
  const createTableQuery = `
    CREATE TABLE IF NOT EXISTS Soil (
      id INT AUTO_INCREMENT PRIMARY KEY,
      moisture FLOAT NOT NULL,
      timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    )
  `;
  db.query(createTableQuery, (err, result) => {
    if (err) {
      throw err;
    }
    console.log('Table created or exists already');
  });
});

// Middleware to parse JSON data
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// Serve static files from the public directory
app.use(express.static(path.join(__dirname, 'public')));

// Example route to test server
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Route to handle data from ESP32
app.post('/soil-data', (req, res) => {
  const { moisture } = req.body;
  const insertQuery = 'INSERT INTO Soil (moisture) VALUES (?)';

  db.query(insertQuery, [moisture], (err, result) => {
    if (err) {
      return res.status(500).send('Error saving data: ' + err);
    }
    res.status(201).send('Data saved successfully');
  });
});

// Route to fetch soil data
//note: req is not used here because we do not process 
//data from the client only send data to client
app.get('/soil-data', (req, res) => {
  const fetchQuery = 'SELECT * FROM Soil ORDER BY timestamp DESC LIMIT 10'; // retrives the lates 10 readings

  db.query(fetchQuery, (err, results) => {
    if (err) { // returns error message in the case of failure
      return res.status(500).send('Error fetching data: ' + err);
    }
    res.json(results); // parses the result and returns data to client
  });
});

// Start the server
app.listen(port, () => { // this communicates with the server at assigned port 
  console.log(`Server is running on port ${port}`);
});
//postman