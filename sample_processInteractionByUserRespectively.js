const { Client, GatewayIntentBits, ModalBuilder, ButtonBuilder, ActionRowBuilder, ButtonStyle } = require('discord.js');
const { event1, event2, event3 } = require('./events.js')

const client = new Client({
  intents: [
	GatewayIntentBits.Guilds,
	GatewayIntentBits.GuildMessages,
	GatewayIntentBits.MessageContent,
  ],
});

const EventType = {
  INTERACTION: 1,
  API: 2,
};

client.on('ready', () => {
  console.log(`Logged in as ${client.user.tag}`);
});

client.on('messageCreate', async (message) => {
	/*
	if (message.content === '!start_create_gamble') {
		const buttons = new ActionRowBuilder()
			.addComponents(
				new ButtonBuilder()
					.setLabel('start creating gamble')
					.setCustomId('startCreatingGamble')
					.setDisabled(false)
					.setStyle(ButtonStyle.Primary),
			)

	await message.channel.send({
		content: '',
		components: [buttons],
		ephemeral: false
	})
	  }
	  */
})

client.on('interactionCreate', async (interaction) => {
	handleEvent(interaction);
});

const events = new Map();

class eventInteraction {
	constructor(event) {
	  this.type = EventType.INTERACTION;
	  this.eventId = event.customId;
	  this.expectedDataType = EventType.INTERACTION;
	  this.action = async (interaction) => {
		await event.action(interaction);
	  };
	  this.response = async (interaction) => {
		await event.response(interaction);
	  };
	  this.if_allowBack = false;
	}
}
  
class eventAPI {
	constructor(event) {
		this.type = EventType.API;
		this.eventId = event.id;
		this.expectedDataType = undefined;
		this.action = event.action;
		this.response = event.response;
		this.if_allowBack = false;
	}
}

class processEvent {
	constructor(userId, seriesInstructions) {
	  this.cur_step = 0;
	  this.userId = userId;
	  this.aipId = 0; // tmp
	  this.series = seriesInstructions;//you gatta arrange properties uniformly
	  this.triggeredEvent = this.series[Object.keys(seriesInstructions)[0]];
	  this.triggeredEventId = this.triggeredEvent.eventId;
	}

  proceedToNext(param) {//->command
	  if (!this.if_expectingResult(param)) {
		  return;
	  }
	  const waitingEvent = this.getWaitingEvent();
	  this.execute_response(waitingEvent);
	  
	  this.cur_step++;
	  const nextWaitingEvent = this.getWaitingEvent();
	  this.execute_act(nextWaitingEvent);
  }

  execute_response(event) {
	event.response;
  }

  execute_act(event) {
	event.action;
  }

  if_expectingResult(param) {
	  if (this.series[this.cur_step].expectedDataType === INTERACTION) {
		  if (if_interaction(param)) {//if param is interaction, it has message
			  return false;
		  }
	  }
	  return true;
  }

  getWaitingEvent() {
	return this.series[Object.keys(seriesInstructions)[this.cur_step]];
  }
/*
  displayCurStatus() {}
  timeout() {}
  */
}

function if_interaction(param) {
  return param.message;
}

async function handleEvent(param) {
	let expectedUserId = undefined;
	let expectedEventId = undefined;
	
	if (if_interaction(param)) {
	  expectedUserId = param.user.id;
	  expectedEventId = param.customId;
	}
	// add branch when event occurred by others
	addEvent(expectedUserId, expectedEventId);
	console.log(events);
	for (const [userId, event] of events.entries()) {
		console.log(userId, expectedUserId, expectedEventId);
		if (userId === expectedUserId && event.waitingEvent && event.getWaitingEvent().eventId === expectedEventId) {
			console.log(true);
			userEvent.proceedToNext(param);
	  	}
		console.log(false);
	}
  }
  
  function addEvent(expectedUserId, expectedEventId) {
	if (!events.has(expectedUserId)) {
	  events.set(expectedUserId, []);
	}
  
	for (const event of instances_events) {
	  if (event.triggeredEventId === expectedEventId) {
		const cloneEvent = Object.assign(Object.create(Object.getPrototypeOf(event)), event);
		cloneEvent.userId = expectedUserId;
		events.get(expectedUserId).push(cloneEvent);
		break;
	  }
	}
	
  }
const interactoinEvent1 = new event1();
const interactoinEvent2 = new event2();
const interactoinEvent3 = new event3();

const commandInteractoinEvent1 = new eventInteraction(interactoinEvent1);
const commandInteractoinEvent2 = new eventInteraction(interactoinEvent2);
const commandInteractoinEvent3 = new eventInteraction(interactoinEvent3);

//const event = processEvent(id, {commandCreateGamble, commandSendGambleModal});
instances_events = [
  new processEvent(undefined, { commandInteractoinEvent1, commandInteractoinEvent2, commandInteractoinEvent3 })]
