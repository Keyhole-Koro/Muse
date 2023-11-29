const { ModalBuilder, MessageActionRow, TextInputBuilder, TextInputStyle, ActionRowBuilder} = require('discord.js');
const { eventInteraction, eventAPI } = require('./utils.js')

class startCreatingGamble extends eventInteraction {
	constructor() {
	  const customId = 'startCreatingGamble';
	  super(customId);
	}
	async action(interaction) {
		const start = new ButtonBuilder()
			.setCustomId(this.customId)
			.setLabel('start creating gamble')
			.setStyle(ButtonStyle.Primary);

		const row = new ActionRowBuilder().addComponents(start);

		await interaction.reply({
			content: ' ',
			components: [row],
		});
	}

	async response(interaction) {
		if (interaction.customId != this.customId) return;
		await interaction.reply('lets get started');
	}
}

class createGamble extends eventInteraction {
	constructor () {
		const customId = 'create_gamble';
		super(customId);
	}
	async action(interaction) {
		const modal = new ModalBuilder()
			.setCustomId(this.customId)
			.setTitle('create gamble');

		//make this doesnt approve type other than number
		const stake = new TextInputBuilder()
			.setCustomId('your_stake')
			.setLabel("Enter your stake")
			.setStyle(TextInputStyle.Short);

		const firstActionRow = new ActionRowBuilder().addComponents(stake);

		modal.addComponents(firstActionRow);

		await interaction.showModal(modal);
	}

	async response(interaction) {
		if (interaction.customId != this.customId) return;
		const stakeValue = parseInt(interaction.values[0]);

		if (isNaN(stakeValue)) {
			// Input is not a valid number
			await interaction.reply('Invalid input. Please enter a valid number for your stake.');
			return;
		}

		await interaction.reply(`You chose: ${stakeValue}`);
	}
}

class sendGambleModal extends eventInteraction {
	constructor () {
		const customId = 'startCreatingGamble';
		super(customId);
	}

	async action(interaction) {
		const  customId = 'stake';
		const modal = new ModalBuilder()
					.setCustomId(this.customId)
					.setTitle('modal window for betting');

			//make this doesnt approve type other than number
			const stake = new TextInputBuilder()
					.setCustomId('your_stake')
					.setLabel("Enter your stake")
					.setStyle(TextInputStyle.Short);

			const firstActionRow = new ActionRowBuilder().addComponents(stake);

			modal.addComponents(firstActionRow);

			await interaction.showModal(modal);
	}

	async respon(interaction) {
		if (interaction.customId != this.customId) return;
		await interaction.reply(`you chose ${interaction.label}`);
	}
}

module.exports = {
	startCreatingGamble,
	createGamble,
	sendGambleModal
}