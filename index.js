const { Client, Events, GatewayIntentBits } = require('discord.js');
const { token, testerId} = require('./config.json');
const insertMsg = require('./database/json_insert_message.json');

const guildId = testerId;

const client = new Client({
	intents: [
		GatewayIntentBits.Guilds,
		GatewayIntentBits.GuildMessages,
		GatewayIntentBits.MessageContent,
		GatewayIntentBits.GuildMembers,
	],
});


const makeVCComnd = require('./commands/makeVC.js');

client.once(Events.ClientReady, async () => {
	console.log(`Muse is ready\n`);
	await client.application.commands.set(makeVCComnd.data, guildId);
});

client.on('messageCreate', message => {
    if (message.author.bot) {
        return;
    }
	insertMsg.insert(message);
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