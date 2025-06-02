
import { useState } from "react";
import { invoke } from "@tauri-apps/api/tauri";

function App() {
  const [t1, setT1] = useState("Ctrl+C");
  const [act1, setAct1] = useState(500);
  const [rt1, setRt1] = useState(true);

  const [t2, setT2] = useState("Ctrl+V");
  const [act2, setAct2] = useState(500);
  const [rt2, setRt2] = useState(false);

  const [status, setStatus] = useState("");

  const sendConfig = async () => {
    try {
      await invoke("send_config_to_device", {
        t1, act1, rt1: rt1 ? 1 : 0,
        t2, act2, rt2: rt2 ? 1 : 0,
      });
      setStatus("✅ Configuración enviada");
    } catch (err) {
      setStatus("❌ Error: " + err);
    }
  };

  return (
    <div style={{ padding: "2rem" }}>
      <h1>Keypad Configurator</h1>
      <div>
        <h2>Tecla 1</h2>
        <input value={t1} onChange={(e) => setT1(e.target.value)} />
        <input type="range" min="0" max="1023" value={act1} onChange={(e) => setAct1(Number(e.target.value))} />
        <label><input type="checkbox" checked={rt1} onChange={(e) => setRt1(e.target.checked)} /> Rapid Trigger</label>
      </div>
      <div>
        <h2>Tecla 2</h2>
        <input value={t2} onChange={(e) => setT2(e.target.value)} />
        <input type="range" min="0" max="1023" value={act2} onChange={(e) => setAct2(Number(e.target.value))} />
        <label><input type="checkbox" checked={rt2} onChange={(e) => setRt2(e.target.checked)} /> Rapid Trigger</label>
      </div>
      <button onClick={sendConfig}>Guardar configuración</button>
      <p>{status}</p>
    </div>
  );
}

export default App;
