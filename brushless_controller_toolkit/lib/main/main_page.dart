import 'package:brushless_controller_toolkit/main/main_bloc.dart';
import 'package:brushless_controller_toolkit/main/main_event.dart';
import 'package:brushless_controller_toolkit/main/main_state.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:flutter_libserialport/flutter_libserialport.dart';

class MainPage extends StatelessWidget {
  const MainPage({super.key});

  @override
  Widget build(BuildContext context) {
    return BlocProvider(
      create: (context) => MainBloc(),
      child: BlocBuilder<MainBloc, MainState>(
        builder: (context, state) {
          final bloc = context.read<MainBloc>();
          final theme = Theme.of(context);

          return Scaffold(
            appBar: AppBar(
              title: const Text('Brushless Controller Toolkit'),
            ),
            body: Padding(
              padding: const EdgeInsets.symmetric(horizontal: 16),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: <Widget>[
                  const Divider(),
                  Row(
                    children: [
                      // Connect to the COM port
                      TextButton(
                        onPressed:
                            state.selectedSerialPort == null || state.isLoading
                                ? null
                                : () {
                                    bloc.add(const MainEventConnect());
                                  },
                        child: state.isConnected
                            ? const Text('Disconnect')
                            : const Text('Connect'),
                      ),
                      const Spacer(),
                      MenuBar(
                        children: [
                          SubmenuButton(
                            menuChildren: state.serialPorts
                                .map((serialPort) => MenuItemButton(
                                      child: Text(_serialPortName(serialPort)),
                                      onPressed: () {
                                        bloc.add(
                                          MainEventSelectComPort(serialPort),
                                        );
                                      },
                                    ))
                                .toList(),
                            trailingIcon: const Icon(Icons.arrow_drop_down),
                            child: Text(
                              state.selectedSerialPort == null
                                  ? 'Select COM port'
                                  : _serialPortName(state.selectedSerialPort!),
                            ),
                            onOpen: () => bloc.add(
                              const MainEventSearchComPort(),
                            ),
                          ),
                          // Refresh available COM ports
                          IconButton(
                            onPressed: () => bloc.add(
                              const MainEventSearchComPort(),
                            ),
                            icon: const Icon(Icons.refresh),
                          ),
                        ],
                      ),
                    ],
                  ),
                ],
              ),
            ),
          );
        },
      ),
    );
  }

  static String _serialPortName(SerialPort serialPort) =>
      "${serialPort.name} - ${serialPort.description}";
}
