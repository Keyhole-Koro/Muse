const Discord = require('discord.js');
const database = require('../json_handler.js');
const utils = require('.../utils');
const deal_mention = require('../mention/deal_mention.js');
const info_user_handler = require('../user/json_user_handler.js');

async function detect_spam(path_log_msg) {
	const db = new database(path_log_msg, id_channel, removeChannel);
}

function rate_malicious(user) {
	const rate = 0;
	const maliciousMentionScore = deal_mention.evaluatePreviousMentions(user);
	const machingRate = ;
	return rate;
}

function construct_hash_msg(path_log_msg) {
	const db = new database(path_log_msg, id_channel, removeChannel);
	const hashValue = utils.hashStringToNumber(readMessage());
	db.modifyData("123456", { "message": { "hash": hash }});
}

async function makeUserSilent(guild, author, penalty_time) {
  info_user_handler.saveRoles(author);
  const role_name = "silent" + penalty_time.toString();

  try {
    let role = guild.roles.cache.find(role => role.name === role_name);
    if (!role) {
      role = await utils.makeSilentRole(guild, role_name, [{ SEND_MESSAGES: false }]);
    }
    author.roles.add(role);

    setTimeout(() => {
      author.roles.remove(role);
	  info_user_handler.restoreRoles(author);
      console.log(`Removed role ${role.name} from ${author.username}`);
    }, penalty_time);
  } catch(error) {
    console.error('Error:', error);
  }
}