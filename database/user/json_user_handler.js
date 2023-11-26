const client = require('../../index.js');

function updateScore(db, user, score) {
    console.log("1");
    db.modifyData(user, { "score_mention": score });
    console.log("2");
}

function saveRoles(db, guild, member) {
    try {
        const roles = member.roles.cache.array();
        const roleIDs = roles.map(role => role.id);

        db.modifyData(user.id, { "roleIDs": roleIDs });

    } catch (error) {
        console.error('Error as to fetching roles of a member or inserting them to :', error);
    }

}

function restoreRoles(db, guild, member) {
    try {
        const foundData = db.findMatchingData(user.id);

        const rolesToAdd = foundData.roleIDs;

        member.roles.add(rolesToAdd);

    } catch (error) {
        console.error('Error as to fetching a member or adding roles:', error);
    }
}

module.exports = {
    updateScore,
    saveRoles,
    restoreRoles
}