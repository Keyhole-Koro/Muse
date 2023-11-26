const fs = require('fs');
const database = require('../../json_handler.js');
const utils = require('../../utils.js');

//make this handle the interval of mentions and save the result not to read everytime
//add deleteData() but be carefull i, if an object is removed, the following objects will be shifted, dataMatch is copied one, be careful not toremove from it
function evaluatePreviousMentions(db, user) {
    console.log(user);
    const dataMatch = db.findMatchingData(user.id);
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
    return score;
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

/*
const date = utils.constructDate(0, 12);

const arrangedObject = constructMentions(user, mention, date);

const db = new database('./log_mentions.json', utils.user_id, utils.ifDue);

console.log(evaluatePreviousMentions(db, user));
//db.removeExpiredObject();

//db.insertData(arrangedObject);
//db.modifyData("123456", { "due": "2023-12-25" });
//db.deleteData("123456");
*/
module.exports = evaluatePreviousMentions;