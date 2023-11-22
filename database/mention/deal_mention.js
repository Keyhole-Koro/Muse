const database = require('../json_handler.js');
const utils = require('.../utils');

const user_id = function(data, expectedId) {
	const filteredData = data.filter(item => item.user.id === expectedId);
	return filteredData;
}

//make this handle the interval of mentions and save the result not to read everytime
function evaluatePreviousMentions(user) {
	const indicesDataMatch = db.findMachingIndex(user.id);
	const score = 0;
	const basic_penalty = 40;
	if (utils.ifAdmin(user)) {
		penalty = 25;
	}
	for (int i = 0; i < numElements(indicesDataMatch); i++) {
		const data = db.getData(indicesDataMatch[i]);
		//add deleteData() but be carefull i, if an object is removed, the following objects will be shifted
		if (!utils.ifUserMatch(data.user, exprectedUser) || isDue(utils.parse_dateStr(data.date.due))) {
			continue;
		}
		data.mention.mentions.forEach(mention => 
			{
			if (utils.ifRoleMention(mention)) {
				score += score + basic_penalty;
				continue;
			}
			score += score + basic_penalty-25;
			}
		)
	}
	return utils.scoreToPercentage(score);
}

function constructMentions(user, mention, date) {
	const arrangedObject = {
		"user": {
			"id": user.id || -1,
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
	"id": 1,
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

module.exports = evaluatePreviousMentions;