const database = require('../json_handler.js');
const utils = require('.../utils');

const id_channel = function(data, expectedId) {
	const index = data.findIndex(item => item.channel.id === expectedId);
	return index;
}

const disableChannel = function(index) {
	
}

function constructChannel(type, channel, user, category, date, tags) {
	const arrangedChannel = {
		"type": type || "",
		"channel": {
			"id": channel.id || -1,
			"name": channel.name || ""
		},
		"category": {
			"id": category.id || -1,
			"name": category.name || ""
		},
		"user": {
			"id": user.id || -1,
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

const channel = {
	"id": 123456,
	"name": "Channel Name"
};

const category = {
	"id": 1,
	"name": "Sample Category"
};

const user = {
	"id": 789012,
	"name": "Keyhole",
	"name_id": "korokoro"
};

const date = utils.constructDate(1);

const tags = ["tag1", "tag2"];

const arrangedChannel = constructChannel("quickCreatedChannel", channel, user, category, date, tags);

const db = new database('./channel_manage.json', id_channel, utils.ifDue);

db.removeExiredObject();

db.insertData(arrangedChannel);
//db.modifyData("123456", { "due": "2023-12-25" });
//db.deleteData("123456");
