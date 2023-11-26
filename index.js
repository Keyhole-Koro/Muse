const { Client, Events, GatewayIntentBits } = require('discord.js');
const { token, testerId} = require('./config.json');
const utils = require('./utils.js');

const json_db = require('./json_handler.js');
const evaluatePreviousMentions = require('./database/mention/deal_mention.js');
const user_handler = require('./database/user/json_user_handler.js')
const anti_spam = require('./database/anti_spam/anti_spam.js')

const guildId = testerId;

const client = new Client({
	intents: [
		GatewayIntentBits.Guilds,
		GatewayIntentBits.GuildMessages,
		GatewayIntentBits.MessageContent,
		GatewayIntentBits.GuildMembers,
	],
});

const json_info_user = new json_db('./database/user/info_user.json', utils.user_id, utils.ifDue);
const json_channel = new json_db('./database/channel/channel_manage.json', utils.user_id, utils.ifDue);
const json_mention = new json_db('./database/mention/log_mentions.json', utils.user_id, utils.ifDue);
const json_role = new json_db('./database/user/role_user_tmp.json', utils.user_id, utils.dummy_removelMethod);

const makeVCComnd = require('./commands/makeVC.js');

client.once(Events.ClientReady, async () => {
	console.log(`Muse is ready\n`);
	await client.application.commands.set(makeVCComnd.data, guildId);
});

client.on('messageCreate', message => {
    if (message.author.bot) {
        return;
    }
	const user = utils.constructUserFromMsgAuthor(message.author);
	if (utils.num_mentions(message) > 0) {
		//test not yet
		const score = evaluatePreviousMentions(json_mention, user);
		console.log(score);
		if (score >= 100) {
			anti_spam.makeUserSilent(json_role, message.guild, user, 10);
		}
		user_handler.updateScore(json_info_user, user, score);
	}
});

client.on(Events.InteractionCreate, async interaction => {
	
	if (!interaction.isChatInputCommand()) return;

	//if (interaction.commandName === makeVCComnd.data.name) {
	if (interaction.commandName === 'makevc') {

		try {
			await makeVCComnd.execute(interaction);
		} catch (error) {
			console.error(error);
			if (interaction.replied || interaction.deferred) {
				await interaction.followUp({ content: 'excecution failed', ephemeral: true });
			} else {
				await interaction.reply({ content: 'excecution failed', ephemeral: true });
			}
		}
	} else {
		console.error(`${interaction.commandName} the command doesn't exist`);
	}
});

client.login(token);

module.exports = client;