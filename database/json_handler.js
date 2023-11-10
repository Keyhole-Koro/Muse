const fs = require('fs');
const utils = require('../utils');

function isString(param) {
    return param instanceof String || typeof param === 'string';
}

//add some exeptions
class database {
	constructor(referentPath, referentProperty, removalMethod) {
		this.referentPath = referentPath;
		this.referentProperty = referentProperty;
		this.data = this.readDataFromFile();
		this.removalMethod = removalMethod;
	}

	readDataFromFile() {
		try {
			const data = fs.readFileSync(this.referentPath, 'utf8');
			return JSON.parse(data);
		} catch (error) {
			console.error('Error reading data from file:', error);
			return [];
		}
	}

	writeDataToFile(newObject) {
		try {
			const jsonData = JSON.stringify(newObject, null, 2);
			fs.writeFileSync(this.referentPath, jsonData);
			console.log('Data written to file successfully.');
		} catch (error) {
			console.error('Error writing data to file:', error);
		}
	}

	insertData(newObject) {
		try {
			this.data.push(newObject);
			this.writeDataToFile(this.data);
		} catch (error) {
			console.error('Error writing data to file:', error);
		}
	}

	modifyData(expectedId, newData) {
		const index = this.referentProperty(this.data, expectedId);
		if (index !== -1) {
			this.data[index] = { ...this.data[index], ...newData };
			this.writeDataToFile(this.data);
			console.log(`Object with ID ${expectedId} modified successfully.`);
		} else {
			console.log(`ID ${expectedId} not found.`);
		}
	}

	deleteData(expectedId) {
		const newData = this.data.filter(item => item.channel.id !== expectedId);
		if (this.data.length !== newData.length) {
			this.writeDataToFile(newData);
			console.log(`Object with ID ${expectedId} deleted successfully.`);
		} else {
			console.log(`Object with ID ${expectedId} not found.`);
		}
	}
	
	removeExpiredChannels() {
		const cur_time = utils.JP_date();
		this.removalMethod(this.data);
		this.writeDataToFile(this.data);
	}
}

module.exports = database;