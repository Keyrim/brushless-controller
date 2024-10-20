import 'package:equatable/equatable.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter_libserialport/flutter_libserialport.dart';

class MainEvent extends Equatable {
  const MainEvent();

  @override
  @mustCallSuper
  List<Object?> get props => [];
}

/// Sent when we want to refresh the list of available COM ports
class MainEventSearchComPort extends MainEvent {
  const MainEventSearchComPort();
}

/// Sent when we want to select a COM port
class MainEventSelectComPort extends MainEvent {
  final SerialPort serialPort;

  const MainEventSelectComPort(this.serialPort);

  @override
  List<Object?> get props => [
        ...super.props,
        serialPort,
      ];
}

/// Sent when we want to connect to the selected COM port
class MainEventConnect extends MainEvent {
  const MainEventConnect();
}
