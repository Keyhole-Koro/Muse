const mysql = require('mysql');

const { db_host, db_user, db_password, db_name } = require('../config.json');

// Create a connection to the MySQL database
const connection = mysql.createConnection({
    host: db_host,
    user: db_user,
    password: db_password,
    database: db_name
});


connection.connect((err) => {
    if (err) {
        console.error('Error connecting to MySQL database: ' + err.stack);
        return;
    }
    console.log('Connected to MySQL database as id ' + connection.threadId);
});


connection.query('SELECT * FROM users', (error, results, fields) => {
    if (error) throw error;
    console.log('SELECT Result:', results);
});

const newUser = { username: 'john_doe', email: 'john@example.com' };
connection.query('INSERT INTO users SET ?', newUser, (error, results, fields) => {
    if (error) throw error;
    console.log('New user inserted with ID:', results.insertId);
});

const userIdToUpdate = 1;
const updatedEmail = 'newemail@example.com';
connection.query('UPDATE users SET email = ? WHERE id = ?', [updatedEmail, userIdToUpdate], (error, results, fields) => {
    if (error) throw error;
    console.log('User updated successfully');
});

const userIdToDelete = 2;
connection.query('DELETE FROM users WHERE id = ?', userIdToDelete, (error, results, fields) => {
    if (error) throw error;
    console.log('User deleted successfully');
});

connection.end((err) => {
    if (err) {
        console.error('Error closing MySQL connection: ' + err.stack);
        return;
    }
    console.log('MySQL connection closed');
});
