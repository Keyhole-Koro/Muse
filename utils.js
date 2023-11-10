const { ChannelType, Permissions } = require('discord.js');

//const highestPermission = {allow: Permissions.ALL}

module.exports = {
	JP_date: function() {
	const current_date = new Date().toLocaleString({ timeZone: 'Asia/Tokyo' });
	console.log(current_date);
	const dateParts = current_date.replace(",", "").split(" ");
	const dateStr = dateParts[0];
	const timeStr = dateParts[1];

	const [month, day, year] = dateStr.split("/");
	const [hour, minute, second] = timeStr.split(":");
	//the month parameter in the Date constructor is zero-based, meaning January is represented as 0, February as 1, March as 2, and so on.
	const date_current_date = new Date(+year, +month - 1, +day, +hour, +minute, +second);

	return date_current_date;
	},
	
	parse_dateStr: function(dateString) {
		const dateParts = dateString.split("T");
		const dateStr = dateParts[0];
		const timeStr = dateParts[1];

		const replacedTimeStr = timeStr.replace("Z", "");
		const [month, day, year] = dateStr.split("-");
		const [hour, minute, second] = replacedTimeStr.split(":");

		const parsedDate = new Date(+year, +month, +day, +hour, +minute, +second);

		return parsedDate;
	},
	
	createVoiceChannel: async function(interaction, guild, channelName, parentCategory) {
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
	},
	
	hashStringToNumber: function(inputString) {
		const chunkSize = 10;
		const charOffset = 'a'.charCodeAt(0);

		let hashValue = 0;
		for (let i = 0; i < inputString.length; i += chunkSize) {
			const chunk = inputString.slice(i, i + chunkSize);
			let chunkHash = 0;

			for (let j = 0; j < chunk.length; j++) {
				const charCode = chunk.charCodeAt(j) - charOffset;
				chunkHash = (chunkHash << 5) - chunkHash + charCode;
			}

			hashValue += chunkHash;
		}

		return hashValue;
	},
}
