const { ChannelType, Permissions } = require('discord.js');

//const highestPermission = {allow: Permissions.ALL}

async function createVoiceChannel(interaction, guild, channelName, parentCategory) {
	try {
		const voiceChannel = await guild.channels.create({
			name: channelName,
			type: ChannelType.GuildVoice,
			parent: parentCategory,
		});
		//await voiceChannel.permissionOverwrites.edit(interaction.user, {VIEW_CHANNEL : true});
		return voiceChannel;
		
	} catch (error) {
		console.error('Error creating voice channel:', error);
		return null;
	}
}

module.exports = createVoiceChannel;
