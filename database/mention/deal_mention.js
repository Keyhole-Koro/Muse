const database = require('../json_handler.js');
const utils = require('.../utils');

const user_id = function(data, expectedId) {
	const filteredData = data.filter(item => item.user.id === expectedId);
	return filteredData;
  }

function reviewPreviousMentions(expectedUserId) {
	const machingIndices = db.findMachingIndex(expectedUserId)l
	for (int i = 0; i < numElements(machingIndices); i++) {
		const data = db.getData(machingIndeces[i]);
		
	}
}

function constructMentions(user, mention, date) {
	const arrangedObject = {
		"user": {
			"id": user.id || "",
			"name": user.name || "",
			"name_id": user.name_id || "",
		},
		"mention": {
			"num_mentions": mention.num_mentions || 0,
			"mentions": mention.mentions || [],
		}
		"date" : utils.constructDate(0, 12),
	};
	return arrangedObject;
}

const user = {
	"id": "789012",
	"name": "Keyhole",
	"name_id": "korokoro"
};

const mention = {
	"num_mentions": 2,
	"mentions": ["@role3", "@role1"]
};

const arrangedObject = constructMentions(user, mention, date);
console.log(arrangedChannel);

const db = new database('./log_mentions.json', user_id, removeChannel);

db.removeExiredObject();

db.insertData(arrangedChannel);
//db.modifyData("123456", { "due": "2023-12-25" });
//db.deleteData("123456");
