class AlignConfig:
    def __init__(self, match=1, mismatch=-1, gap_open=-1, gap_extend=-1, top=False, left=False, right=False,
                 bottom=False, lower_diagonal=-2147483648,
                 upper_diagonal=2147483647, local=False):
        self.match = match
        self.mismatch = mismatch
        self.gap_open = gap_open
        self.gap_extend = gap_extend
        self.top = top
        self.left = left
        self.right = right
        self.bottom = bottom
        self.lower_diagonal = lower_diagonal
        self.upper_diagonal = upper_diagonal
        self.local = local
