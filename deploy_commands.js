const { REST, Routes } = require('discord.js');

const greetCmnd = require('./commands/makeVC.js');

const { applicationId, guildId, token } = require('./config.json');

const commands = [greetCmnd.data.toJSON()];

const rest = new REST({ version: '10' }).setToken(token);

(async () => {
    try {
        await rest.put(
            Routes.applicationGuildCommands(applicationId, guildId),
            { body: commands },
        );
        console.log('the command is set');
    } catch (error) {
        console.error('an error happened while setting the command:', error);
    }
})();
