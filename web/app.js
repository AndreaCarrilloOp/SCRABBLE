'use strict';

const BOARD_SIZE = 15;
const MAX_WORD_LENGTH = 6;
const BONUS_CLASSES = {
  '3P': 'p3',
  '2P': 'p2',
  '3L': 'l3',
  '2L': 'l2',
  '**': 'center'
};

const $ = id => document.getElementById(id);
const cells = [];
let engine;
let state;
let revealed = false;

function status(message, error = false) {
  $('status').textContent = message;
  $('status').classList.toggle('error', error);
}

function call(name, types = [], args = []) {
  return JSON.parse(engine.ccall('scrabble_' + name, 'string', types, args));
}

function appendCoordinate(value) {
  const coordinate = document.createElement('span');
  coordinate.className = 'coord';
  coordinate.textContent = value;
  $('board').append(coordinate);
}

function highlight() {
  const row = Number($('row').value);
  const col = Number($('col').value);
  const valid = Number.isInteger(row) && Number.isInteger(col) &&
    row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;

  cells.forEach((cell, index) => {
    const selected = !!state?.started && !state.finished && valid &&
      index === row * BOARD_SIZE + col;
    cell.classList.toggle('selected', selected);
    cell.setAttribute('aria-pressed', String(selected));
  });
}

function createBoard() {
  for (let col = 0; col < BOARD_SIZE; col++) appendCoordinate(col);
  appendCoordinate('');

  for (let row = 0; row < BOARD_SIZE; row++) {
    for (let col = 0; col < BOARD_SIZE; col++) {
      const cell = document.createElement('button');
      cell.type = 'button';
      cell.className = 'cell';
      cell.append(document.createElement('span'));
      cell.addEventListener('click', () => {
        $('row').value = row;
        $('col').value = col;
        highlight();
      });
      cells.push(cell);
      $('board').append(cell);
    }
    appendCoordinate(row);
  }
}

function renderNames() {
  const previous = [...$('names').querySelectorAll('input')]
    .map(input => input.value);
  $('names').replaceChildren();

  for (let index = 0; index < Number($('count').value); index++) {
    const label = document.createElement('label');
    const input = document.createElement('input');
    label.textContent = 'Jugador ' + (index + 1);
    input.maxLength = 30;
    input.value = previous[index] || '';
    input.placeholder = label.textContent;
    label.append(input);
    $('names').append(label);
  }
}

function renderBoard() {
  state.board.forEach((tile, index) => {
    const cell = cells[index];
    const row = Math.floor(index / BOARD_SIZE);
    const col = index % BOARD_SIZE;
    cell.className = 'cell ' + (BONUS_CLASSES[tile.bonus] || '') +
      (tile.letter ? ' occupied' : '');
    cell.firstChild.textContent = tile.letter;
    cell.setAttribute('aria-label',
      'Fila ' + row + ', columna ' + col + ', ' +
      (tile.letter || tile.bonus || 'vacía'));
  });
  highlight();
}

function renderScores() {
  const top = Math.max(...state.players.map(player => player.score));
  const winners = state.players.filter(player => player.score === top)
    .map(player => player.name);

  $('turn').textContent = state.finished
    ? (winners.length > 1 ? 'Empate: ' : 'Ganador: ') + winners.join(', ')
    : 'Turno de ' + state.players[state.active].name;
  $('bag').textContent = 'Bolsa: ' + state.bag + ' fichas · Pases: ' + state.passes;

  $('scores').replaceChildren();
  state.players.forEach((player, index) => {
    const row = document.createElement('div');
    const name = document.createElement('span');
    const score = document.createElement('span');
    row.className = 'score' + (!state.finished && index === state.active ? ' active' : '');
    name.textContent = player.name;
    score.textContent = player.score + ' pts';
    row.append(name, score);
    $('scores').append(row);
  });
}

function renderRack() {
  $('rack').replaceChildren();
  if (!revealed || state.finished) return;

  state.rack.forEach(tile => {
    const button = document.createElement('button');
    const points = document.createElement('small');
    button.type = 'button';
    button.className = 'tile';
    button.textContent = tile.letter;
    points.textContent = tile.points;
    button.append(points);
    button.addEventListener('click', () => {
      const word = $('word');
      if (word.value.length < MAX_WORD_LENGTH) word.value += tile.letter;
      word.focus();
    });
    $('rack').append(button);
  });
}

function render() {
  renderBoard();
  $('setup').hidden = state.started;
  $('game').hidden = !state.started;
  if (!state.started) return;

  renderScores();
  $('history').textContent = state.history || 'Sin jugadas.';
  $('handoff').hidden = state.finished || revealed;
  $('controls').hidden = state.finished || !revealed;
  $('finish').hidden = state.finished;
  $('restart').hidden = !state.finished;
  renderRack();
}

function action(name, types = [], args = []) {
  try {
    const result = call(name, types, args);
    state = result.state;
    if (result.ok && result.advanced) {
      revealed = false;
      $('word').value = '';
      $('letters').value = '';
    }
    render();
    status(result.message, !result.ok);
  } catch (error) {
    status('Error del motor: ' + error.message, true);
  }
}

function bindEvents() {
  $('count').addEventListener('change', renderNames);
  $('row').addEventListener('input', highlight);
  $('col').addEventListener('input', highlight);

  $('start').addEventListener('submit', event => {
    event.preventDefault();
    const names = [...$('names').querySelectorAll('input')]
      .map(input => input.value.trim());
    while (names.length < 4) names.push('');
    action('start', ['number', 'string', 'string', 'string', 'string'],
      [Number($('count').value), ...names]);
  });

  $('reveal').addEventListener('click', () => {
    revealed = true;
    render();
  });

  $('play').addEventListener('submit', event => {
    event.preventDefault();
    action('play', ['number', 'number', 'number', 'string'], [
      Number($('row').value),
      Number($('col').value),
      Number($('direction').value),
      $('word').value.toUpperCase()
    ]);
  });

  $('exchange').addEventListener('submit', event => {
    event.preventDefault();
    action('exchange', ['string'], [$('letters').value.toUpperCase()]);
  });

  $('pass').addEventListener('click', () => action('pass'));
  $('finish').addEventListener('click', () => {
    if (confirm('¿Finalizar partida?')) action('finish');
  });
  $('restart').addEventListener('click', () => {
    $('game').hidden = true;
    $('setup').hidden = false;
    revealed = false;
  });
}

async function initialize() {
  try {
    engine = await createScrabble();
    state = call('state').state;
    render();
    $('startButton').disabled = false;
    status('Motor C++ listo. Inicia una partida.');
  } catch (error) {
    status('No se pudo cargar WebAssembly: ' + error.message, true);
  }
}

createBoard();
renderNames();
bindEvents();
initialize();
