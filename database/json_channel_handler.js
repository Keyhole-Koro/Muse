const database = require('./json_handler.js');
const utils = require('../utils');

const id_channel = function(data, expectedId) {
	const index = data.findIndex(item => item.channel.id === expectedId);
	return index;
}

const removeChannel = function(data) {
	const cur_time = utils.JP_date();

	return data.filter(item => {
		const due = utils.parse_dateStr(item.date.due);
		const differenceComperedToDue = (due - cur_time)/(60*60*1000);
		console.log(differenceComperedToDue);
		return differenceComperedToDue >= 0;
	});
};

const disableChannel = function(index) {
	
}

function constructChannel(type, channel, user, category, date, tags) {
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
		"user": {
			"id": user.id || "",
			"name": user.name || "",
			"name_id": user.name_id || "",
		},
		"date" : {
			"created_at": date.created_at || "",
			"due": date.due || undefined,
		},
		"available": true,
		"tags": tags || undefined
	};

	return arrangedChannel;
}

//you can pass 0 as parameters
function constructDate(daysToAdd, hoursToAdd) {
	const currentDate = utils.JP_date();

	const dueDate = new Date(currentDate);
	
	if (daysToAdd !== undefined && daysToAdd !== null && daysToAdd > 0) {
		dueDate.setDate(currentDate.getDate() + daysToAdd);
	}

	if (hoursToAdd !== undefined && hoursToAdd !== null && hoursToAdd > 0) {
		dueDate.setHours(currentDate.getHours() + hoursToAdd);
	}

	const date = {
		"created_at": currentDate,
		"due": dueDate,
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

const user = {
	"id": "789012",
	"name": "Keyhole",
	"name_id": "korokoro"
};

const date = constructDate(1);

const tags = ["tag1", "tag2"];

const arrangedChannel = constructChannel("quickCreatedChannel", channel, user, category, date, tags);
console.log(arrangedChannel);

const db = new database('./channel_manage.json', id_channel, removeChannel);

db.removeExpiredChannels();

db.insertData(arrangedChannel);
//db.modifyData("123456", { "due": "2023-12-25" });
//db.deleteData("123456");
