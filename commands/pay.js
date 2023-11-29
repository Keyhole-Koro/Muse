const { SlashCommandBuilder } = require('discord.js');

const payComnd = new SlashCommandBuilder()
.setName('pay')
.setDescription('Enter the name_id you want to pay for')
.addStringOption(option =>
    option.setName('name_id')
        .setDescription('Enter the name_id you want to pay for')
        .setRequired(true));

module.exports = {
    data: payComnd.toJSON(),
    async execute(interaction) {
        const guild = interaction.guild;

        try {
            const nameId = interaction.options.getString('name_id');

            await interaction.reply('Payment successful.');
        } catch (error) {
            console.error('Error:', error);
            await interaction.reply('Failed to pay.');
        }
    },
};
