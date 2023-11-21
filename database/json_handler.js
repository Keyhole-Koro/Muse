const fs = require('fs');
const utils = require('../utils');
const readline = require('readline');

/*
someday, ill make it extract each line by step, once you find due not passed, stop the program
not to do excess sorting
*/

function isString(param) {
    return param instanceof String || typeof param === 'string';
}

function isNumber(param) {
    return typeof param === 'number';
}

//add some exeptions
class database {
	constructor(referentPath, referentPropertyToFind, removalMethod) {
		this.referentPath = referentPath;
		this.referentPropertyToFind = referentPropertyToFind;
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

	findMatchingIndex(expectedId) {
		const foundElements = this.referentPropertyToFind(this.data, expectedId);
		if (isNumber(foundElements)) {
			//even if the above variable is a number, make it a list
			return [foundElements];
		}
		return foundElements;
	}

	getData(index) {
		return this.data[index];
	}

	modifyData(expectedId, newData) {
		const indices = this.findMatchingIndex(expectedId);
		
		if (indices.length > 0) {
		  indices.forEach(index => {
			this.data[index] = { ...this.data[index], ...newData };
		  });
	  
		  this.writeDataToFile(this.data);
		  console.log(`Objects with ID ${expectedId} modified successfully.`);
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

	removeExpiredObject() {
		const cur_time = utils.JP_date();
		this.removalMethod(this.data);
		this.writeDataToFile(this.data);
	}
}

module.exports = database;