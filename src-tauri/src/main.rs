
use std::io::Write;
use serialport::SerialPort;
use tauri::command;

#[command]
fn send_config_to_device(
    t1: String, act1: u16, rt1: u8,
    t2: String, act2: u16, rt2: u8
) -> Result<(), String> {
    let ports = serialport::available_ports().map_err(|e| e.to_string())?;
    let port = ports.into_iter().find(|p| p.port_name.contains("tty") || p.port_name.contains("COM"))
        .ok_or("No se encontró ningún dispositivo conectado")?;

    let mut port = serialport::new(port.port_name, 9600)
        .timeout(std::time::Duration::from_secs(1))
        .open()
        .map_err(|e| e.to_string())?;

    let command = format!("CFG;T1={},ACT={},RT={};T2={},ACT={},RT={}
",
        t1, act1, rt1, t2, act2, rt2);

    port.write_all(command.as_bytes()).map_err(|e| e.to_string())?;
    Ok(())
}
