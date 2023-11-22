const fs = require('fs');
const database = require('../json_handler.js');
const utils = require('../../utils.js');

const user_id = function(data, expectedId) {
	const filteredData = data.filter(item => item.user.id === expectedId);
	return filteredData;
}

const removeObject = function(data) {
	const cur_time = utils.JP_date();

	return data.filter(item => {
		const due = utils.parse_dateStr(item.date.due);
		const differenceComperedToDue = (due - cur_time)/(60*60*1000);
		return differenceComperedToDue >= 0;
	});
};

//make this handle the interval of mentions and save the result not to read everytime
//add deleteData() but be carefull i, if an object is removed, the following objects will be shifted, dataMatch is copied one, be careful not toremove from it
function evaluatePreviousMentions(user) {
    const dataMatch = db.findMatchingIndex(user.id);
    let score = 0;
    const basic_penalty = 30;

    if (utils.ifAdmin(user)) {
        basic_penalty = 15;
    }

    for (const data of dataMatch) {
        if (!utils.ifUserMatch(data.user, user) || utils.isDue(utils.parse_dateStr(data.date.due))) {
            continue;
        }
        data.mention.mentions.forEach(mention => {
            if (utils.ifRoleMention(mention)) {
                score += basic_penalty;
            } else {
                score += basic_penalty - 15;
            }
        });
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
		},
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
	"mentions": ["role3", "role1"]
};

const date = utils.constructDate(0, 12);

const arrangedObject = constructMentions(user, mention, date);
console.log(arrangedObject);

const db = new database('./log_mentions.json', user_id, removeObject);

console.log(evaluatePreviousMentions(user));
//db.removeExpiredObject();

//db.insertData(arrangedObject);
//db.modifyData("123456", { "due": "2023-12-25" });
//db.deleteData("123456");

module.exports = evaluatePreviousMentions;