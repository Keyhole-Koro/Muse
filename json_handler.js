const fs = require('fs');
const utils = require('./utils');

/*
someday, ill make it extract each line by step, once you find due not passed, stop the program
not to do excess sorting
*/

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

	findMatchingData(expectedUser) {
		const foundElements = this.referentPropertyToFind(this.data, expectedUser.id);
		if (utils.isNumber(foundElements)) {
			//even if the above variable is a number, make it a list
			return [foundElements];
		}
		return foundElements;
	}

	getData(index) {
		return this.data[index];
	}

	modifyData(expectedUser, newData) {
		const foundData = this.findMatchingData(expectedUser.id);
	
		if (foundData.length > 0) {
			foundData.forEach(data => {
				data = { ...data, ...newData };
			});
	
			this.writeDataToFile(this.data);
			console.log(`Objects ${expectedUser} modified successfully.`);
		} else {
			console.log(`${expectedUser} not found.`);
		}
	}

	deleteData(index) {
		if (this.data.length !== data[index].length) {
			this.writeDataToFile(data[index]);
			console.log(`Object with ID ${expectedId} deleted successfully.`);
		} else {
			console.log(`Object with ID ${expectedId} not found.`);
		}
	}
	
	removeExpiredObject() {
		this.data = this.data.filter((obj) => !this.removalMethod(obj));
		this.writeDataToFile(this.data);
	}
}

module.exports = database;