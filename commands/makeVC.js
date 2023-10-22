const { SlashCommandBuilder } = require('discord.js');
const { vc_category } = require('./config.json');

const createVoiceChannel = require('../utils.js');

module.exports = {
    data: new SlashCommandBuilder()
        .setName('makeVC')
        .setDescription('This is for making voice channel, disappears in 24 hours')
        .addStringOption(option =>
            option.setName('vcname')
            .setDescription('Enter the voice channel name you want to create')
            .setRequired(true)),
    async execute(interaction) {
        //make sure cooltime
        
        const guild = interaction.guild;
        const vcName = interaction.options.getString('vcname');

        // Create a voice channel
        try {
            const voiceChannel = await createVoiceChannel(guild, vcName, vc_category)
            await interaction.reply(`Voice channel "${voiceChannel.name}" created successfully! It will disappear in 24 hours.`);
            
            //replace here to use databse
            setTimeout(async () => {
                await voiceChannel.delete();
                console.log(`Voice channel "${voiceChannel.name}" deleted after 24 hours.`);
            }, 24 * 60 * 60 * 1000);
            
        } catch (error) {
            console.error('Error creating voice channel:', error);
            await interaction.reply('Failed to create the voice channel.');
        }
    },
};
