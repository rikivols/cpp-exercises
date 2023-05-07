# Floyd Warshall Algorithm in python


# The number of vertices
nV = 5

INF = 999


# Algorithm implementation
def floyd_warshall(G):
    distance = list(map(lambda i: list(map(lambda j: j, i)), G))

    # Adding vertices individually
    for k in range(nV):
        for i in range(nV):
            for j in range(nV):
                distance[i][j] = min(distance[i][j], distance[i][k] + distance[k][j])
    return distance


# Printing the solution
def print_solution(distance):
    for i in range(nV):
        for j in range(nV):
            if(distance[i][j] == INF):
                print("INF", end=" ")
            else:
                print(distance[i][j], end="  ")
        print(" ")

def convert(adj, V):

    # Initialize a matrix
    matrix = [[INF for j in range(V)]
              for i in range(V)]

    for i in range(V):
        for j in adj[i]:
            matrix[i][j] = 1

    return matrix

def printList(adj, V):

    for i in range(V):
        print(i, end = '')

        for j in adj[i]:
            print(' --> ' + str(j), end = '')

        print()

    print()

def convert_to_numbers(lst):
    string_pairs = {}
    counter = 0
    for k in lst:
        if k not in string_pairs:
            string_pairs[k] = counter
            counter += 1
        for val in lst[k]:
            if val not in string_pairs:
                string_pairs[val] = counter
                counter += 1

    print(string_pairs)
    res = []

    for i, k in enumerate(string_pairs):
        res.append([])
        if k in lst:
            for val in lst[k]:
                res[-1].append(string_pairs[val])

    return res

#
# G = [[0, 3, INF, 5],
#      [2, 0, INF, 4],
#      [INF, 1, 0, INF],
#      [INF, INF, 2, 0]]
# floyd_warshall(G)
if __name__ == "__main__":
    lst = {
        "C++": ["Pascal", "Java"],
        "Pascal": ["Basic", "PHP"],
        "Java": ["PHP", "Pascal"],
        "PHP": ["Basic"]
    }

    # lst = {
    #     "Python": ["PHP"],
    #     "PHP": ["Perl"],
    #     "Perl": ["Bash"],
    #     "Bash": ["Javascript"],
    #     "Javascript": ["VBScript"],
    # }


    nums = convert_to_numbers(lst)
    print(nums)
    matrix = convert(nums, nV)
    print(matrix)
    distances = floyd_warshall(matrix)
    print(distances)

    dct = {'Python': 0, 'PHP': 1, 'Perl': 2, 'Bash': 3, 'Javascript': 4, 'VBScript': 5}
    used_values = set()
    for v1 in distances:
        cur_val = 0
        for v2 in v1:
            if v2 != INF:
                cur_val += 1
        if cur_val in used_values:
            print("wrong")
        used_values.add(cur_val)
