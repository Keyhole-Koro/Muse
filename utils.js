async function createVoiceChannel(guild, channelName, parentCategory) {
    try {
        const voiceChannel = await guild.channels.create(channelName, {
            type: 'GUILD_VOICE',
            parent: parentCategory,
            permissionOverwrites: [
                // You can add permission overwrites for roles or members here if needed
            ],
        });
        return voiceChannel;
    } catch (error) {
        console.error('Error creating voice channel:', error);
        return null;
    }
}

module.exports = createVoiceChannel;
