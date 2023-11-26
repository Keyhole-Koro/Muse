const fs = require('fs');
const { ChannelType, Permissions } = require('discord.js');
const client = require('./index.js');

//const highestPermission = {allow: Permissions.ALL}

module.exports = {
	ifAdmin: function(user) {
		try {
			const data = fs.readFileSync('./config.json', 'utf8');
			const json_data = JSON.parse(data);
			const admins = json_data.administrators;

			for (const admin of admins) {
				if (admin.id === user.id || admin.name_id === user.name_id) {
					return true;
				}
			}
			return false; 
		} catch (error) {
			console.error('Error reading data from file:', error);
			return false;
		}
	},

	isString: function(param) {
		return param instanceof String || typeof param === 'string';
	},
	
	isNumber: function(param) {
		return typeof param === 'number';
	},

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

	constructDate: function(daysToAdd, hoursToAdd) {
		const currentDate = this.JP_date();
	
		const dueDate = new Date(currentDate);
		
		if (daysToAdd !== undefined && daysToAdd !== null && daysToAdd > 0) {
			dueDate.setDate(currentDate.getDate() + daysToAdd);
		}
	
		if (hoursToAdd !== undefined && hoursToAdd !== null && hoursToAdd > 0) {
			dueDate.setHours(currentDate.getHours() + hoursToAdd);
		}
	
		const date = {
			"created_at": currentDate,
			"due": dueDate,
		};
	
		return date;
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

	calculateHoursDifferenceFromNow: function(previous_date) {
		const currentDate = new Date();
		const timeDifference = parseInt((currentDate - previous_date) / (1000 * 60 * 60));
		console.log("Time Difference: " + timeDifference + " hours");
		return timeDifference;
	},

	isDue: function(due) {
		if (this.calculateHoursDifferenceFromNow(due) > 0) {
			return true;
		}
		return false;
	},

	numElements: function(list_) {
		return list_.length;
	},
	
	ifStrNumber: function(c) {
		if (!this.isString(c)) {
			throw new Error("invalid type of parameter at ifStrNumber()")
		}
		if (c >= '0' && c <= '9') {
			return true;
		}
		return false;
	},

	ifRoleMention: function(mention_str) {
		for (let i = 0; i < mention_str.length; i++) {
			const c = mention_str.substring(i);
			if (!this.ifStrNumber(c)) {
				return false;
			}
		}
		return true;
	},

	ifUserMatch: function(user1, user2) {
		if (user1.id === user2.id || user1.name_id === user2.name_id) {
			return true;
		}
		return false;
	},
	
	scoreToPercentage: function(score) {
		if (score >= 100) {
			return 100;
		}
		return score;
	},

	ifDue: function(data) {
		return data.filter(item => {
			const due = utils.parse_dateStr(item.date.due);
			return this.calculateHoursDifferenceFromNow(due) >= 0;
		});
	},

	dummy_removalMethod: function(data) {
		return false;
	},

	user_id: function(data, expectedId) {
		const filteredData = data.filter(item => item.user.id === expectedId);
		return filteredData;
	},

	num_mentions: function(message) {
		return message.mentions.users.size + message.mentions.roles.size;
	},

	constructUserFromMsgAuthor: function(author) {
		console.log(author);
		const user = {
			"name": author.globalName || "",
			"id": author.id || -1,
			"name_id": author.username || ""
		}
		return user;
	},
	  
	  makeSilentRole: async function(guild, role_name, permission) {
		try {
		  return await guild.roles.create({
			data: {
			  name: role_name,
			  permissions: permission,
			},
		  });
	  
		  console.log(`Created role ${role.name} with id ${role.id}`);
		} catch(error) {
		  console.error('Failed to create a new role:', error);
		}
	  }
	
}
