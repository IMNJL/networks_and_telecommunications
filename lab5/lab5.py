def node_to_coords(node, rows, cols):
    
    """
    
    Преобразование номера узла в координаты (row, col, layer)
    """
    layer = node // (rows * cols)
    rem = node % (rows * cols)
    row = rem // cols
    col = rem % cols
    return row, col, layer


def coords_to_node(row, col, layer, rows, cols):
    """
    Преобразование координат в номер узла
    """
    return layer * rows * cols + row * cols + col


def find_route(src, dst, rows, cols):
    """
    Построение маршрута:
    сначала по строке, затем по столбцу
    """
    route = [src]
    details = []

    src_r, src_c, src_l = node_to_coords(src, rows, cols)
    dst_r, dst_c, dst_l = node_to_coords(dst, rows, cols)

    # Движение по строке
    if src_r != dst_r:
        intermediate = coords_to_node(dst_r, src_c, src_l, rows, cols)
        route.append(intermediate)
        details.append(
            f"Узел {src} -> Узел {intermediate}: Передача по строке {dst_r} (плоскость {src_l})"
        )
        src = intermediate

    # Движение по столбцу
    if src_c != dst_c:
        route.append(dst)
        details.append(
            f"Узел {src} -> Узел {dst}: Передача по столбцу {dst_c} (плоскость {src_l})"
        )

    return route, details


def main():
    print("Введите количество строк (M): ", end="")
    rows = int(input())

    print("Введите количество столбцов (N): ", end="")
    cols = int(input())

    print("Введите количество плоскостей (L): ", end="")
    layers = int(input())

    total_nodes = rows * cols * layers

    print("\nСеть создана. Общее число узлов:", total_nodes)
    print(f"Диапазон допустимых адресов: от 0 до {total_nodes - 1}")

    while True:
        print("\n" + "=" * 45)
        print("1. Найти маршрут")
        print("2. Выйти")

        choice = input("Выберите действие: ")

        if choice == "2":
            break

        if choice != "1":
            print("Неверный выбор")
            continue

        src = int(input("Введите адрес источника: "))
        dst = int(input("Введите адрес приемника: "))

        if not (0 <= src < total_nodes and 0 <= dst < total_nodes):
            print("Ошибка: адрес вне допустимого диапазона")
            continue

        src_coords = node_to_coords(src, rows, cols)
        dst_coords = node_to_coords(dst, rows, cols)

        route, details = find_route(src, dst, rows, cols)

        print("\n--- Результат ---")
        print(f"Источник: {src} {src_coords}")
        print(f"Приемник: {dst} {dst_coords}")
        print("Маршрут:", " -> ".join(map(str, route)))
        print("L_WAY:", len(route))

        print("\nДетализация маршрута:")
        for d in details:
            print(d)


if __name__ == "__main__":
    main()
