# Simulated Rivian car for Openpilot sim, adapted from simulated_car.py
import traceback
import cereal.messaging as messaging

from opendbc.can.packer import CANPacker
from opendbc.can.parser import CANParser
from openpilot.common.params import Params
from openpilot.selfdrive.pandad.pandad_api_impl import can_list_to_can_capnp
from openpilot.tools.sim.lib.common import SimulatorState
from openpilot.opendbc_repo.opendbc.car.rivian.riviancan import create_lka_steering, create_longitudinal, create_wheel_touch, create_adas_status
from openpilot.opendbc_repo.opendbc.car.rivian.values import RivianSafetyFlags

class SimulatedRivianCar:
  """Simulates a Rivian R1 vehicle (panda state + can messages) to OpenPilot"""
  packer = CANPacker("rivian_primary_actuator")

  def __init__(self):
    self.pm = messaging.PubMaster(['can', 'pandaStates'])
    self.sm = messaging.SubMaster(['carControl', 'controlsState', 'carParams', 'selfdriveState'])
    self.cp = self.get_car_can_parser()
    self.idx = 0
    self.params = Params()
    self.obd_multiplexing = False
    self.acm_lka_hba_cmd = {"ACM_hbaSysState": 0, "ACM_hbaLamp": 0, "ACM_hbaOnOffState": 0, "ACM_slifOnOffState": 0}  # Dummy values
    self.sccm_wheel_touch = {"SCCM_WheelTouch_Counter": 0, "SCCM_WheelTouch_HandsOn": 0, "SCCM_WheelTouch_CapacitiveValue": 0, "SETME_X52": 0}  # Dummy
    self.vdm_adas_status = {"VDM_AdasStatus_Checksum": 0, "VDM_AdasStatus_Counter": 0, "VDM_AdasDecelLimit": 0, "VDM_AdasDriverAccelPriorityStatus": 0, "VDM_AdasFaultStatus": 0, "VDM_AdasAccelLimit": 0, "VDM_AdasDriverModeStatus": 0, "VDM_AdasUnkown1": 0, "VDM_AdasInterfaceStatus": 1, "VDM_AdasVehicleHoldStatus": 0, "VDM_UserAdasRequest": 0}  # Dummy

  @staticmethod
  def get_car_can_parser():
    dbc_f = 'rivian_primary_actuator'
    checks = []
    return CANParser(dbc_f, checks, 0)

  def send_can_messages(self, simulator_state: SimulatorState):
    if not simulator_state.valid:
      return

    msg = []

    # Rivian CAN messages - adapt from riviancan.py
    # Steering (LKA)
    apply_torque = int(simulator_state.steering_torque * 250)  # Scale torque
    enabled = simulator_state.is_engaged
    active = simulator_state.is_engaged
    msg.append(create_lka_steering(self.packer, self.idx, self.acm_lka_hba_cmd, apply_torque, enabled, active))

    # Wheel touch every 5 frames
    if self.idx % 5 == 0:
      msg.append(create_wheel_touch(self.packer, self.sccm_wheel_touch, enabled))

    # Longitudinal if enabled
    if True:  # Assume longitudinal enabled for sim
      accel = float(np.clip(simulator_state.accel, -3.5, 2.0))
      msg.append(create_longitudinal(self.packer, self.idx, accel, enabled))
    else:
      interface_status = 1 if simulator_state.cancel else 0
      msg.append(create_adas_status(self.packer, self.vdm_adas_status, interface_status))

    # Vehicle speed, gear, etc. - simulate basic state
    speed_kph = simulator_state.speed * 3.6
    msg.append(self.packer.make_can_msg("ESP_Status", 0, {"ESP_Vehicle_Speed": speed_kph}))  # vEgo
    msg.append(self.packer.make_can_msg("VDM_PropStatus", 0, {"VDM_Prndl_Status": 4, "VDM_AcceleratorPedalPosition": simulator_state.user_gas}))  # Gear drive, gas
    msg.append(self.packer.make_can_msg("ESPiB3", 0, {"ESPiB3_pMC1": simulator_state.user_brake * 250}))  # Brake pressure
    msg.append(self.packer.make_can_msg("EPAS_AdasStatus", 0, {"EPAS_InternalSas": simulator_state.steering_angle, "EPAS_SteeringAngleSpeed": simulator_state.steering_rate}))
    msg.append(self.packer.make_can_msg("EPAS_SystemStatus", 0, {"EPAS_TorsionBarTorque": simulator_state.user_torque}))
    msg.append(self.packer.make_can_msg("RCM_Status", 0, {"RCM_Status_IND_WARN_BELT_DRIVER": 0}))  # Seatbelt latched
    msg.append(self.packer.make_can_msg("IndicatorLights", 1, {"TurnLightLeft": 1 if simulator_state.left_blinker else 0, "TurnLightRight": 1 if simulator_state.right_blinker else 0}))  # Blinkers
    msg.append(self.packer.make_can_msg("SCCM_FEEDBACK", 0, {"MAIN_ON": 1 if simulator_state.ignition else 0}))  # Main on

    self.pm.send('can', can_list_to_can_capnp(msg))

  def send_panda_state(self, simulator_state):
    self.sm.update(0)

    if self.params.get_bool("ObdMultiplexingEnabled") != self.obd_multiplexing:
      self.obd_multiplexing = not self.obd_multiplexing
      self.params.put_bool("ObdMultiplexingChanged", True)

    dat = messaging.new_message('pandaStates', 1)
    dat.valid = True
    dat.pandaStates[0] = {
      'ignitionLine': simulator_state.ignition,
      'pandaType': "blackPanda",
      'controlsAllowed': True,
      'safetyModel': 'rivian',
      'alternativeExperience': self.sm["carParams"].alternativeExperience,
      'safetyParam': RivianSafetyFlags.LONG_CONTROL.value,
    }
    self.pm.send('pandaStates', dat)

  def update(self, simulator_state: SimulatorState):
    try:
      self.send_can_messages(simulator_state)

      if self.idx % 50 == 0:  # 2Hz for panda
        self.send_panda_state(simulator_state)

      self.idx += 1
    except Exception:
      traceback.print_exc()
      raise