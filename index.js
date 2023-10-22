const makeVCComnd = require('./commands/makeVC.js');

const { Client, Events, GatewayIntentBits } = require('discord.js');
const { token } = require('./config.json');
const client = new Client({ intents: [GatewayIntentBits.Guilds] });


client.once(Events.ClientReady, c => {
    console.log(`${c.user.tag} is ready\n`);
});

client.on(Events.InteractionCreate, async interaction => {

    if (!interaction.isChatInputCommand()) return;

    if (interaction.commandName === makeVCComnd.data.name) {
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


// ログインします
client.login(token);
