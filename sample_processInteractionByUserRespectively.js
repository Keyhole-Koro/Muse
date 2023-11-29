const { Client, GatewayIntentBits, ModalBuilder, ButtonBuilder, ActionRowBuilder, ButtonStyle, TextInputBuilder, TextInputStyle } = require('discord.js');
const { startCreatingGamble, createGamble, sendGambleModal } = require('./gamble_creating.js')
const { EventType } = require('./utils.js')

const { token } = require('./config.json');

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
	/*
	if (interaction.commandName === 'pay') {
		const parameter = interaction.options.getString('name_id');
		console.log(parameter);
	}
	if (interaction.customId === 'no1') {
		const modal = new ModalBuilder()
			.setCustomId('a')
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
	if (interaction.customId === 'a') {
		console.log(iteracton.value[0]);
	}
	*/
});

const events = new Map();

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
	  console.log(waitingEvent);
	  this.cur_step++;
	  const nextWaitingEvent = this.getWaitingEvent();
	  this.execute_act(nextWaitingEvent.action);
	  console.log(nextWaitingEvent);
  }

  execute_response(event) {
	event.response;
  }

  execute_act(event) {
	event.action;
  }

  if_expectingResult(param) {
	  if (this.getWaitingEvent().expectedDataType === EventType.INTERACTION) {
		  if (!if_interaction(param)) {//if param is interaction, it has message
			  return false;
		  }
	  }
	  return true;
  }

  getWaitingEvent() {
	return this.series[Object.keys(this.series)[this.cur_step]];
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
	for (const [userId, userEvents] of events.entries()) {
		for (const userEvent of userEvents) {
			if (userId === expectedUserId && userEvent.getWaitingEvent().eventId === expectedEventId) {
				userEvent.proceedToNext(param);
			}
		}
	}
}


  function addEvent(expectedUserId, expectedEventId) {
	if (!events.has(expectedUserId)) {
	  events.set(expectedUserId, []);
	}
	for (const event of instances_events) {
	  if (event.triggeredEventId === expectedEventId) {
		const cloneEvent = new processEvent(expectedUserId, event.series);
		cloneEvent.cur_step = event.cur_step;
		events.get(expectedUserId).push(cloneEvent);
		break;
	  }
	}
  }
const interactionStartCreatingGamble = new startCreatingGamble();
const interactionCreateGamble = new createGamble();
const interactionSendGambleModal = new sendGambleModal();

//const event = processEvent(id, {commandCreateGamble, commandSendGambleModal});
instances_events = [
  new processEvent(undefined, { interactionStartCreatingGamble, interactionCreateGamble, interactionSendGambleModal })]

client.login(token);