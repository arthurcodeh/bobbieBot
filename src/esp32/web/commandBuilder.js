/**
 * État global de l'interface
 */
const state = {
    cmdCount:    0,
    online:      false,
    activeEye:   null,
};



/* ── Log console ────────────────────────────────────────────── */
function log(msg, type = '') {
    const el = document.getElementById('console-log');
    const now = new Date().toLocaleTimeString('fr-FR', { hour: '2-digit', minute: '2-digit', second: '2-digit' });
    const line = document.createElement('div');
    line.className = 'console-line';
    line.innerHTML = `<span class="ts">${now}</span><span class="msg ${type}">${msg}</span>`;
    el.appendChild(line);
    // Garde seulement les 40 dernières lignes
    while (el.children.length > 40) el.removeChild(el.firstChild);
    el.scrollTop = el.scrollHeight;
}

/* ── Envoi d'une commande ──────────────────────────────────── */
/**
 * Envoie une commande série à l'Arduino via la route /cmd?value=...
 * Le format attendu par SerialProtocol est : "member servoIndex angle"
 * ou "member action" pour les LEDs.
 *
 * @param {string} value - Commande texte (ex: "head 0 90", "eyes red")
 */
function cmd(value) {
    const t0 = performance.now();

    fetch('/cmd?value=' + encodeURIComponent(value))
        .then(res => {
            if (!res.ok) throw new Error(`HTTP ${res.status}`);
            return res.text();
        })
        .then(() => {
            const ping = Math.round(performance.now() - t0);
            state.cmdCount++;
            setOnline(true);
            log(`→ ${value}`, 'ok');
            document.getElementById('footer-cmd').textContent = value;
            document.getElementById('footer-ping').textContent = `PING: ${ping}ms`;
            document.getElementById('cmd-count').textContent = `${state.cmdCount} CMD SENT`;
        })
        .catch(err => {
            setOnline(false);
            log(`✕ ${value} — ${err.message}`, 'err');
        });
}

/* ── Yeux — raccourci avec suivi de l'état actif ───────────── */
/**
 * Commande les LEDs yeux et met à jour le bouton actif.
 * @param {string} action - 'off' | 'white' | 'red' | 'green' | 'blue'
 */
function setEye(action) {
    cmd('eyes ' + action);
    document.querySelectorAll('.eye-btn').forEach(b => b.classList.remove('active'));
    const active = document.querySelector(`.eye-btn.${action}`);
    if (active) active.classList.add('active');
    state.activeEye = action;
}

function eyesOff() { setEye('off'); }

/* ── Séquences prédéfinies ──────────────────────────────────── */
/**
 * Remet tous les membres en position neutre (90°).
 * Les commandes sont envoyées avec un petit délai pour éviter
 * la saturation du buffer série de l'Arduino.
 */
function resetAll() {
    const seq = [
        'head 0 90', 'head 1 90',
        'left 0 90', 'left 1 90', 'left 2 45',
        'right 0 90','right 1 90','right 2 135',
    ];
    seq.forEach((c, i) => setTimeout(() => cmd(c), i * 80));

    // Réinitialise les sliders visuellement
    document.querySelectorAll('input[type="range"]').forEach(s => {
        const mid = (parseInt(s.min) + parseInt(s.max)) / 2;
        s.value = Math.round(mid);
    });
    document.getElementById('val-h0').textContent  = '90°';
    document.getElementById('val-h1').textContent  = '90°';
    document.getElementById('val-l0').textContent  = '90°';
    document.getElementById('val-l1').textContent  = '90°';
    document.getElementById('val-l2').textContent  = '45°';
    document.getElementById('val-r0').textContent  = '90°';
    document.getElementById('val-r1').textContent  = '90°';
    document.getElementById('val-r2').textContent  = '135°';
}

/** Séquence de wave du bras gauche */
function waveSequence() {
    const seq = [
        [0, 'left 0 150'],
        [400, 'left 1 150'],
        [800, 'left 1 30'],
        [1200,'left 1 150'],
        [1600,'left 1 30'],
        [2000,'left 0 90'],
        [2400,'left 1 90'],
    ];
    seq.forEach(([t, c]) => setTimeout(() => cmd(c), t));
}

/** Séquence de hochement de tête */
function nodSequence() {
    const seq = [
        [0,   'head 1 60'],
        [600, 'head 1 120'],
        [1200,'head 1 60'],
        [1800,'head 1 90'],
    ];
    seq.forEach(([t, c]) => setTimeout(() => cmd(c), t));
}

;