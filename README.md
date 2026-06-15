# sim_rviz_plugins

Pacote ROS 2 com plugins de RViz usados para facilitar testes da simulacao.

No fluxo atual, ele adiciona um painel no RViz para enviar destinos operacionais
ao `nav_hub`, sem precisar usar o comando manual `ros2 topic pub`.

## Pacote No Workspace

Este pacote deve ficar no mesmo workspace dos pacotes `sim_bot` e `nav_hub`:

```text
~/sim_ws/src/sim_bot
~/sim_ws/src/nav_hub
~/sim_ws/src/sim_rviz_plugins
```

## Painel De Destinos

O plugin principal e:

```text
sim_rviz_plugins/SimDestinationPanel
```

Ele cria um painel no RViz com botoes numerados de `1` a `18`.

Ao clicar em um botao, o painel publica uma mensagem:

```text
topico: /destination
tipo: std_msgs/msg/Int32
valor: numero do botao clicado
```

Exemplo:

```text
Botao 3
  -> publica /destination = 3
```

## QoS

O painel publica `/destination` com o mesmo QoS esperado pelo fluxo da Oregon:

```cpp
rclcpp::QoS(rclcpp::KeepLast(1)).reliable().transient_local()
```

Isso e importante porque o `sim_main_route_graph` do pacote `nav_hub` escuta
`/destination` com QoS `reliable + transient_local`.

Por isso, o painel substitui este teste manual:

```bash
ros2 topic pub --once /destination std_msgs/msg/Int32 "{data: 3}" \
  --qos-durability transient_local \
  --qos-reliability reliable
```

## Fluxo Com nav_hub

Fluxo esperado:

```text
SimDestinationPanel no RViz
  -> publica /destination
  -> nav_hub/sim_main_route_graph
  -> publica /goal_pose
  -> bt_navigator
  -> route_server
  -> grafo em nav_hub/graphs/aceleradoras.json
  -> robo segue a rota
```

Esse painel nao controla o robo diretamente. Ele apenas envia o ID operacional
do destino, simulando a entrada que na Oregon real viria de botoeira, LoRa ou
outro sistema de chamada.

## Como Compilar

Na raiz do workspace:

```bash
cd ~/sim_ws
colcon build --packages-select sim_rviz_plugins
source install/setup.bash
```

Se tambem alterou `sim_bot` ou `nav_hub`:

```bash
cd ~/sim_ws
colcon build --packages-select sim_rviz_plugins nav_hub sim_bot
source install/setup.bash
```

## Como Usar No RViz

1. Suba a simulacao:

```bash
cd ~/sim_ws
source install/setup.bash
ros2 launch sim_bot sim_manager.launch.py
```

2. Em outro terminal, suba o `sim_main_route_graph`:

```bash
cd ~/sim_ws
source install/setup.bash
ros2 launch nav_hub sim_main_route_graph.launch.py
```

3. No RViz:

```text
Panels
  -> Add New Panel
  -> sim_rviz_plugins
  -> SimDestinationPanel
```

4. Clique no destino desejado.

## Arquivos Principais

```text
src/sim_destination_panel.cpp
include/sim_rviz_plugins/sim_destination_panel.hpp
plugin_description.xml
CMakeLists.txt
package.xml
```

## Observacoes

- O painel publica destinos de `1` a `18`.
- O destino precisa existir no arquivo usado pelo `nav_hub/sim_main_route_graph`.
- O pacote nao deve conter regras de navegacao; ele e apenas uma interface RViz
  para publicar IDs de destino.
