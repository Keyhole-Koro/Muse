const { Client, GatewayIntentBits } = require('discord.js');
const { handleInteraction, processAllUserInteractions } = require('./interactionHandler');

const userInteractions = new Map();

const client = new Client({
  intents: [
    GatewayIntentBits.Guilds,
    GatewayIntentBits.GuildMessages,
    GatewayIntentBits.MessageContent,
  ],
});

client.on('ready', () => {
  console.log(`Logged in as ${client.user.tag}`);
});

client.on('interactionCreate', (interaction) => {
  handleInteraction(interaction);
});

function setInstructions(series) {
  const struct_ = {

  }
  return struct_;
}

processInteractions();

enum {
  INTERACTION = 1,
  API,
}

const createGamble() {
    const 
}

const sendModal() {

}

class presetInteraction(action) {
    construct() {
      type = INTERACTION,
      customId = action.ui.customId,
      expectedDataType = INTERACTION,
      act = action,
      if_allowBack = false;
    }
}

class presetAPI(action) {
    construct() {
      type = API,
      id = ,
      expectedDataType = ,
      act = action,
      if_allowBack = false,
    }
}

class processEvent(userId, seriesInstructions) {
  construct() {
    cur_step = 0,
    userId = userId,
    aipId = 0;//tmp
    series = setInstructions(seriesInstructions),//series
    
  }
  progressOne(param) {

  }
  if_expectingResult() {

  }
  displayCurStatus() {

  }
  timeout() {

  }
}

const commandCreateGamble = presetInteraction(createGamble);
const commandSendModal = presetInteraction(sendModal);


const event = processEvent(id, {commandCreateGamble, commandSendModal, e3});




function handleInteraction(interaction) {
  const buttonActions = {
    'selection0': { action: addInteraction, reply: firstSelection },
    'selection1': { action: addInteraction, reply: secondSelection },
    'selection2': { action: addInteraction, reply: thirdSelection },
    'selection3': { action: addInteraction, reply: processFourInteractions },
    'id4': { action: handleId4 },
  };

  const buttonAction = buttonActions[interaction.buttonCustomId];

  if (buttonAction) {
    buttonAction.action(interaction);
    if (buttonAction.reply) {
      interaction.reply(buttonAction.reply);
    }
  }
}

function addInteraction(interaction) {
  const userId = interaction.user.id;

  if (!userInteractions.has(userId)) {
    userInteractions.set(userId, []);
  }

  userInteractions.get(userId).push(interaction);
}


client.login('YOUR_BOT_TOKEN');