const fs = require('fs');

const id_channel = function(data, expectedId) {
	const index = data.findIndex(item => item.channel.id === expectedId);
	return index;
}
//add some exeptions
class database {
	constructor(referentPath, referentProperty) {
		this.referentPath = referentPath;
		this.referentProperty = referentProperty;
		this.data = this.readDataFromFile();
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
		this.data.push(newObject);
		this.writeDataToFile(this.data);
	}

	modifyData(expectedId, newData) {
		const index = this.referentProperty(this.data, expectedId);
		if (index !== -1) {
			this.data[index] = { ...this.data[index], ...newData }; // <-- Fixed 'data' to 'this.data'
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
}

function constructChannel(channel, maker, category, date_made, due, tags) {
	const arrangedChannel = {
		"channel": {
			"id": channel.id || "",
			"name": channel.name || ""
		},
		"category": {
			"id": category.id || -1,
			"name": category.name || ""
		},
		"maker": {
			"id": maker.id || "",
			"name": maker.name || ""
		},
		"date_made": date_made || "",
		"due": due || undefined,
		"if_exist": true,
		"tags": tags || undefined
	};

	return arrangedChannel;
}

// Example usage:
const channel = {
	"id": "123456",
	"name": "Channel Name"
};

const category = {
	"id": 1,
	"name": "Sample Category"
};

const maker = {
	"id": "789012",
	"name": "Keyhole"
};

const dateMade = "2023-10-25";
const dueDate = "2023-11-25";
const tags = ["tag1", "tag2"];

const arrangedChannel = constructChannel(channel, maker, category, dateMade, dueDate, tags);
console.log(arrangedChannel);

const db = new database('./channel_manage.json', id_channel);

db.insertData(arrangedChannel);
db.modifyData("123456", { "due": "2023-12-25" });
//db.deleteData("123456");
