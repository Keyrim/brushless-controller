import 'package:brushless_controller_toolkit/main/main_event.dart';
import 'package:brushless_controller_toolkit/main/main_state.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:flutter_libserialport/flutter_libserialport.dart';

class MainBloc extends Bloc<MainEvent, MainState> {
  MainBloc() : super(const MainState.initial()) {
    on<MainEventSearchComPort>(_onSearchComPort);
    on<MainEventSelectComPort>(_onSelectComPort);
    on<MainEventConnect>(_onConnect);

    // Load the list of available COM ports
    add(const MainEventSearchComPort());
  }

  Future<void> _onSearchComPort(
    MainEventSearchComPort event,
    Emitter<MainState> emit,
  ) async {
    emit(state.copyWithLoading());

    // Get the list of available COM ports
    final serialPortNames = SerialPort.availablePorts;
    final serialPorts = serialPortNames
        .map(
          (name) => SerialPort(name),
        )
        .toList();

    emit(state.copyWithComPorts(serialPorts));
  }

  Future<void> _onSelectComPort(
    MainEventSelectComPort event,
    Emitter<MainState> emit,
  ) async {
    emit(state.copyWithSelectedSerialPort(event.serialPort));
  }

  Future<void> _onConnect(
    MainEventConnect event,
    Emitter<MainState> emit,
  ) async {
    // Connect to the selected COM port
    final serialPort = state.selectedSerialPort;
    if (serialPort == null) {
      return;
    }

    final isOpen = serialPort.open(
      mode: SerialPortMode.readWrite,
    );

    // if(isOpen){
    //   serialPort.read(bytes)
    // }

    emit(state.copyWithConnected(isOpen));
  }
}
