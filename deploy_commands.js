const { REST } = require('discord.js');
const { Routes } = require('discord-api-types/v10');
const { applicationId, testerId, token } = require('./config.json');

const guildId = testerId;

const makeVCComnd = require('./commands/makeVC.js');
const payComnd = require('./commands/pay.js');
console.log(payComnd.data);
const commands = [makeVCComnd.data, payComnd.data];
const rest = new REST({ version: '10' }).setToken(token);

(async () => {
    try {
        await rest.put(
            Routes.applicationGuildCommands(applicationId, guildId),
            { body: commands }
        );
        console.log('The commands are set');
    } catch (error) {
        console.error('An error occurred while setting the commands:', error);
    }
})();
