###  nxp

+ 预约驾驶
冬天室外 / 冬天车库
夏天室外 / 夏天车库
  - zone left dds
    1. 舱内温度         in         CABIN_TEMPERATURE  topic: cabin_msgs::msg::TOPIC_CABIN_TEMPERATURE = "cabin_msgs/msg/Temperature";     struct: std_msgs::msg::Temperature
    2. 环境温度         in         ENV_OUTSIDE_TEMPERATURE     topic:  hvac_msgs::msg::TOPIC_ENV_OUT_TEMPERATURE      struct: std_msgs::msg::Temperature
    3. 空调 AC          in/out     HVAC_AC_ON / HVAC_SET_AC_ON  hvac_msgs::msg::TOPIC_AC / hvac_msgs::srv::TOPIC_SET_AC            struct: hvac_msgs::msg::Ac  
    4. 空调 温度        in/out     HVAC_TEMPERATURE_CURRENT / HVAC_TEMPERATURE_SET  hvac_msgs::msg::TOPIC_HVAC_CURRENT_TEMPERATURE /  hvac_msgs::srv::TOPIC_HVAC_SET_TEMPERATURE     struct: std_msgs::msg::Temperature
    5. 方向盘加热       in&out     HVAC_STEERING_WHEEL_HEAT  / HVAC_SET_STEERING_WHEEL_HEAT   hvac_msgs::msg::TOPIC_STEERING_WHEEL_HEAT / hvac_msgs::srv::TOPIC_SET_STEERING_WHEEL_HEAT

  - zone right dds
    1. 座椅通风         in/out     SEAT_VENTILATE / SEAT_SET_VENTILATE   seat_msgs::msg::TOPIC_SEAT_VENTILATE   / seat_msgs::srv::TOPIC_SET_SEAT_VENTILATE        seat_msgs::msg::Ventilate
    2. 座椅加热         in/out     SEAT_HEAT / SEAT_SET_HEAT   seat_msgs::msg::TOPIC_SEAT_HEAT   /   seat_msgs::srv::TOPIC_SET_SEAT_HEAT        seat_msgs::msg::Heat
    3. 座椅压力传感器   in         SEAT_OCCUPANCY            seat_msgs::msg::TOPIC_SEAT_OCCUPANCY  seat_msgs::msg::Occupancy

  - nxp can
    1. 电池预热  目标温度 23-27   can out    BATTERY_PREHEAT
    2. 电池剩余电量               can in     BATTERY_REMAINING_POWER  ?%    battery_msgs::msg::TOPIC_BATTERY_REMAINING_POWER   battery_msgs::msg::RemainingPower
    3. 未来五天的最低温 通过CAN 发送到 bms  (dds in/can out) BATTERY_FORCAST_MIN_TEMP
  - hu            
    1. 预约驾驶时间                       APPOINTMENT_DRIVE    drive_msgs::srv::TOPIC_SET_APPOINTMENT_DRIVE    drive_msgs::srv::AppointmentDrive
    2. 未来五天的最高温最低温   dds               WEATHER_FORCAST_TEMP              cloud_msgs::msg::TOPIC_WEATHER_FORCAST_TEMPERATURE             cloud_msgs::msg::WeatherForcastTemperature

