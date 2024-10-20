import 'package:equatable/equatable.dart';
import 'package:flutter_libserialport/flutter_libserialport.dart';

class MainState extends Equatable {
  /// True if this state is loading
  final bool isLoading;

  /// List of available COM ports
  final List<SerialPort> serialPorts;

  /// Current [SerialPort] selected
  final SerialPort? selectedSerialPort;

  /// True when connected to the selected COM port
  final bool isConnected;

  const MainState({
    required this.isLoading,
    required this.serialPorts,
    required this.selectedSerialPort,
    this.isConnected = false,
  });

  const MainState.initial()
      : isLoading = false,
        serialPorts = const [],
        selectedSerialPort = null,
        isConnected = false;

  /// Creates a copy of this state with some fields updated
  MainState copyWith({
    bool? isLoading,
    List<SerialPort>? serialPorts,
    SerialPort? selectedSerialPort,
    bool? isConnected,
  }) =>
      MainState(
        isLoading: isLoading ?? this.isLoading,
        serialPorts: serialPorts ?? this.serialPorts,
        selectedSerialPort: selectedSerialPort ?? this.selectedSerialPort,
        isConnected: isConnected ?? this.isConnected,
      );

  /// Create to loading state
  MainState copyWithLoading() => copyWith(
        isLoading: true,
      );

  /// Create a copy with the given COM ports
  MainState copyWithComPorts(List<SerialPort> serialPorts) => copyWith(
        serialPorts: serialPorts,
        isLoading: false,
      );

  /// Create a copy with the given selected COM port
  MainState copyWithSelectedSerialPort(SerialPort? selectedSerialPort) =>
      copyWith(
        selectedSerialPort: selectedSerialPort,
      );

  /// Create a copy with the given connection state
  MainState copyWithConnected(bool isConnected) => copyWith(
        isConnected: isConnected,
      );

  @override
  List<Object?> get props => [
        isLoading,
        serialPorts,
        selectedSerialPort,
        isConnected,
      ];
}
