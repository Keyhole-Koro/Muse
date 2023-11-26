const client = require('../../index.js');

function updateScore(db, user, score) {
    db.modifyData(user, { "score_mention": score });
}

function saveRoles(db, guild, user) {
    try {
        const member = guild.members.cache.get(user.id);

        const roles = member.roles.cache.array();
        const roleIDs = roles.map(role => role.id);

        db.modifyData(user.id, { "roleIDs": roleIDs });

    } catch (error) {
        console.error('Error as to fetching roles of a member or inserting them to :', error);
    }

}

function restoreRoles(db, guild, user) {
    try {
        const member = guild.members.cache.get(user.id);

        const foundData = db.findMatchingData(user.id);

        const rolesToAdd = foundData.roleIDs;

        await member.roles.add(rolesToAdd);

    } catch (error) {
        console.error('Error as to fetching a member or adding roles:', error);
    }
}
