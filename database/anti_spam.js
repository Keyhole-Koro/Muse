const database = require('./json_handler.js');
const utils = require('../utils');

async function detect_spam(path_log_msg) {
	const db = new database(path_log_msg, id_channel, removeChannel);
}

function percent_match() {
	const rate = 0;
	const num_mentions = ;
	const machingRate = ;
	rate = rate + (10 ** num_mentions) / 2;
	return rate;
}

function num_mentions(message) {
	return message.mentions.users.size + message.mentions.roles.size;
}

function construct_hash_msg(path_log_msg) {
	const db = new database(path_log_msg, id_channel, removeChannel);
	const hashValue = utils.hashStringToNumber(readMessage());
	db.modifyData("123456", { "message": { "hash": hash }});
}

