from collections import defaultdict
from numpy import array

get_trait = {
    "age":    lambda g:  (g & 0x00001f) <<  3,
    "hunger": lambda g: ((g & 0x0000e0) >>  5) + 2,
    "horny":  lambda g: ((g & 0x000f00) >>  8) / 32,
    "aggro":  lambda g: ((g & 0x00f000) >> 12) / 32,
    "mutate": lambda g: ((g & 0x1f0000) >> 16) / 32,
}

def timeseries(s, g):
    return array([di.get(g, 0) for di in s])

def trait_distribution(s, t):
    series = []
    for step in s:
        tr = defaultdict(lambda: 0)
        series.append(tr)
        for g, n in step.items():
            tr[get_trait[t](g)] += n

    return series

def read_stats(f, normalize = True):
    with open(f) as ff:
        series = []
        for step in ff.read().strip().split("\n\n"):
            if normalize: n0 = 0

            d = {}
            series.append(d)
            for line in step.strip().split("\n"):
                g, n = map(int, line.split())
                d[g] = n
                if normalize: n0 += n

            if normalize:
                for g in d:
                    d[g] /= n0

        return series
