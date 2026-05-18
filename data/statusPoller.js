/* ── Statut de connexion ────────────────────────────────────── */
function setOnline(online) {
    if (state.online === online) return;
    state.online = online;
    const dot   = document.getElementById('conn-dot');
    const label = document.getElementById('conn-label');
    dot.className   = 'dot ' + (online ? 'online' : 'offline');
    label.textContent = online ? 'ONLINE' : 'OFFLINE';
    log(online ? '✓ ESP32 joignable' : '✕ ESP32 injoignable', online ? 'ok' : 'err');
}

/* ── Polling des données Arduino (/data) ─────────────────────── */
/**
 * Interroge /data toutes les 2 secondes pour afficher
 * le dernier message renvoyé par l'Arduino (debug série).
 */
//function pollArduinoData() {
//    fetch('/data')
//        .then(res => res.text())
//        .then(data => {
//            if (!data || data === state.lastArduinoData) return;
//            state.lastArduinoData = data;
//            document.getElementById('ard-data').textContent = '▶ ' + data;
//            log('Arduino : ' + data, 'ard');
//            setOnline(true);
//        })
//        .catch(() => setOnline(false));
//}

//setInterval(pollArduinoData, 2000);

/* ── Health check au chargement ─────────────────────────────── */
fetch('/test')
    .then(res => { if (res.ok) setOnline(false); })
    .catch(() => setOnline(false));

log('Interface initialisée', '')