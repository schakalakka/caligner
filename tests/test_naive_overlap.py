import unittest

import pyinterface


class TestNaiveOverlapMethods(unittest.TestCase):
    def test_naive_overlap00(self):
        self.assertEqual(pyinterface.naive_overlap('', ''), 0)

    def test_naive_overlap01(self):
        self.assertEqual(pyinterface.naive_overlap('', 'a'), 0)

    def test_naive_overlap02(self):
        self.assertEqual(pyinterface.naive_overlap('a', ''), 0)

    def test_naive_overlap11(self):
        self.assertEqual(pyinterface.naive_overlap('a', 'a'), 1)

    def test__naive_overlap22(self):
        self.assertEqual(pyinterface.naive_overlap('aa', 'aa'), 2)

    def test__naive_overlap221(self):
        self.assertEqual(pyinterface.naive_overlap('ab', 'ba'), 1)

    def test__naive_overlap331(self):
        self.assertEqual(pyinterface.naive_overlap('bab', 'bba'), 1)

    def test__naive_overlap_30_32_15(self):
        self.assertEqual(
            pyinterface.naive_overlap('gctagtcgatcgtaggtacgctagactgta', 'gtacgctagactgtagctagctagactgcatg'),
            len('gtacgctagactgta'))


if __name__ == '__main__':
    unittest.main()
