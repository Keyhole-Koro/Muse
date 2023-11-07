const fs = require('fs');

function JP_date() {
	const current_date = new Date().toLocaleString({ timeZone: 'Asia/Tokyo' });
	console.log(current_date);
	const dateParts = current_date.replace(",", "").split(" ");
	const dateStr = dateParts[0];
	const timeStr = dateParts[1];

	const [month, day, year] = dateStr.split("/");
	const [hour, minute, second] = timeStr.split(":");
	//the month parameter in the Date constructor is zero-based, meaning January is represented as 0, February as 1, March as 2, and so on.
	const date_current_date = new Date(+year, +month - 1, +day, +hour, +minute, +second);
	console.log(+year, +month, +day, +hour, +minute, +second);
	console.log(date_current_date);
	return date_current_date;
}
//replace date to JP_date

function parseCustomDate(dateString) {
	const dateParts = dateString.split(" at ");
	const dateStr = dateParts[0];
	const timeStr = dateParts[1];

	const months = [
		"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"
	];

	dateStrReplaced = dateStr.replace(",", "");
	const [month, day, year] = dateStrReplaced.split(" ");
	const [hour, minute, second] = timeStr.split(" ")[0].split(":");

	const monthIndex = months.findIndex(m => m.toLowerCase() === month.toLowerCase());
	const parsedDate = new Date(+year, +monthIndex, +day, +hour, +minute, +second);

	return parsedDate;
}

const id_channel = function(data, expectedId) {
	const index = data.findIndex(item => item.channel.id === expectedId);
	return index;
}


const removeChannel = function(data) {
	const cur_time = JP_date();

	return data.filter(item => {
		const due = parseCustomDate(item.date.due);
		const differenceComperedToDue = (due - cur_time)/(60*60*1000);
		console.log(differenceComperedToDue);
		return differenceComperedToDue >= 0;
	});
};

const disableChannel = function(index) {
	
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
		this.data.push(newObject);
		this.writeDataToFile(this.data);
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
		const cur_time = new Date();
		this.removalMethod(this.data);
		this.writeDataToFile(this.data);
	}
}

function constructChannel(type, channel, maker, category, date, tags) {
	const arrangedChannel = {
		"type": type || "",
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
		"date" : {
			"made": date.made || "",
			"due": date.due || undefined,
		},
		"available": true,
		"tags": tags || undefined
	};

	return arrangedChannel;
}

//you can pass 0
function constructDate(daysToAdd, hoursToAdd) {
	const currentDate = new Date();

	const dueDate = new Date(currentDate);
	
	if (daysToAdd !== undefined && daysToAdd !== null && daysToAdd > 0) {
		dueDate.setDate(currentDate.getDate() + daysToAdd);
	}

	if (hoursToAdd !== undefined && hoursToAdd !== null && hoursToAdd > 0) {
		dueDate.setHours(currentDate.getHours() + hoursToAdd);
	}
	
	const options = { year: 'numeric', month: 'long', day: 'numeric', hour: '2-digit', minute: '2-digit', second: '2-digit', timeZoneName: 'short' };
	const made = currentDate.toLocaleDateString('en-US', options);
	const due = dueDate.toLocaleDateString('en-US', options);

	const date = {
		"made": made,
		"due": due,
	};

	return date;
}

function calculateHoursDifferenceFromNow(previous_date) {
	const currentDate = new Date();
	const timeDifference = parseInt((currentDate - previous_date) / (1000 * 60 * 60));
	console.log("Time Difference: " + timeDifference + " hours");
	return timeDifference;
}
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

const date = constructDate(1);

const tags = ["tag1", "tag2"];

const arrangedChannel = constructChannel("quickCreatedChannel", channel, maker, category, date, tags);
console.log(arrangedChannel);

const db = new database('./channel_manage.json', id_channel, removeChannel);

db.removeExpiredChannels();

//db.insertData(arrangedChannel);
//db.modifyData("123456", { "due": "2023-12-25" });
//db.deleteData("123456");
