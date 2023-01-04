def add_product(warehouse, name, price, amount):
    if price < 0:
        return
    product = [name, price, amount, 0]
    warehouse.append(product)


def change_amount(warehouse, name, amount):
    for e in warehouse:
        if e[0] == name:
            e[2] += amount


def change_sold_price(warehouse, name, amount):
    for product in warehouse:
        if product[0] == name:
            product[3] += amount*product[1]


def ship_order(warehouse, shipment):
    for product in shipment:
        change_amount(warehouse, product[0], -product[1])
        change_sold_price(warehouse, product[0], product[1])


def build_matamikya(file, warehouse):
    for line in file:
        words = line.split(' ')
        if words[0] == "add":
            add_product(warehouse, words[2], float(words[3]), float(words[4].strip()))
        if words[0] == "change":
            change_amount(warehouse, words[2], float(words[3]))
        if words[0] == "ship":
            amount_of_words = len(words)
            i = 2
            shipment = []
            while i < amount_of_words:
                name = words[i].split(',')[0]
                shipment.append([name, float(words[i+1])])
                i += 3
            ship_order(warehouse, shipment)


def find_most_expensive_product(warehouse):
    price = 0
    for product in warehouse:
        if product[1] >= price:
            most_expensive = product[0]
            price = product[1]
    return most_expensive


def delete_product_from_warehouse(warehouse, name):
    for i in range(len(warehouse)):
        if warehouse[i][0] == name:
            warehouse.pop(i)
            return


def find_k_most_expensive_products(file_name, k):
    if k <= 0 or file_name == "":
        return []
    warehouse = []
    build_matamikya(file_name, warehouse)
    from copy import deepcopy
    warehouse_copy = deepcopy(warehouse)
    warehouse_copy.sort()
    warehouse_copy.reverse()
    k_most_expensive = []
    i = k
    while i > 0:
        expensive_product = find_most_expensive_product(warehouse_copy)
        k_most_expensive.append(expensive_product)
        delete_product_from_warehouse(warehouse_copy, expensive_product)
        i -= 1
    return k_most_expensive


def find_best_selling_product(file_name):
    warehouse = []
    build_matamikya(file_name, warehouse)
    warehouse.sort()
    warehouse.reverse()
    max_profit = 0
    best_selling = [0, 0]
    for e in warehouse:
        if e[3] >= max_profit:
            max_profit = e[3]
            best_selling[0] = e[0]
            best_selling[1] = e[3]
    return best_selling[0], best_selling[1]

