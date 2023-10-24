const { REST } = require('discord.js');
const { Routes } = require('discord-api-types/v10');
const { applicationId, testerId, token } = require('./config.json');

const guildId = testerId;

const makeVCComnd = require('./commands/makeVC.js');

const commands = [makeVCComnd.data.toJSON()];
const rest = new REST({ version: 'v10' }).setToken(token);

(async () => {
    try {
        await rest.put(
            Routes.applicationGuildCommands(applicationId, guildId),
            { body: commands }
        );
        console.log('The command is set');
    } catch (error) {
        console.error('An error occurred while setting the command:', error);
    }
})();

