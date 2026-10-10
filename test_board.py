"""Run with: python -m unittest test_board.py"""

import os
import tempfile
import unittest
from datetime import datetime, timezone
from pathlib import Path

import board


class BoardStatisticsTests(unittest.TestCase):
    def test_ai_directories_are_excluded_at_every_depth(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            paths = [
                "P1429/P1429.cpp",
                "!P1429/P1429.cpp",
                "！P2482/P2482.cpp",
                "practice/!ai/P1001.cpp",
                "practice/！ai/P1003.cpp",
                "practice/P1008/solution.cpp",
                ".hidden/P1012.cpp",
                "build/P1014.cpp",
            ]
            for relative in paths:
                source = root / relative
                source.parent.mkdir(parents=True, exist_ok=True)
                source.write_text("// source", encoding="utf-8")

            groups = board.scan_problems(root)
            self.assertEqual(set(groups), {"P1429", "P1008"})
            self.assertEqual([file["path"] for file in groups["P1429"]],
                             ["P1429/P1429.cpp"])
            self.assertEqual(board.scan_problems(root / "!P1429"), {})
            self.assertEqual(board.scan_problems(root / "！P2482"), {})

    def test_daily_counts_deduplicate_problems_and_use_china_dates(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            # UTC Feb 28 16:30 is already Feb 29 in China (leap year).
            first = datetime(2024, 2, 28, 16, 30, tzinfo=timezone.utc).timestamp()
            later = datetime(2024, 3, 1, 0, 0, tzinfo=timezone.utc).timestamp()
            sources = {"P1001/first.cpp": first, "P1001/second.py": later,
                       "P1003/P1003.cpp": first}
            for relative, timestamp in sources.items():
                source = root / relative
                source.parent.mkdir(parents=True, exist_ok=True)
                source.write_text("// source", encoding="utf-8")
                os.utime(source, (timestamp, timestamp))

            report = board.build_report(root, board.scan_problems(root), {})
            self.assertEqual(report["total"], 2)
            self.assertEqual(report["source_count"], 3)
            self.assertEqual(report["activity"]["daily_counts"], {"2024-02-29": 2})
            self.assertEqual(sum(report["activity"]["daily_counts"].values()),
                             report["total"])
            problem = next(item for item in report["problems"] if item["id"] == "P1001")
            self.assertEqual(problem["modified_date"], "2024-03-01 08:00")
            self.assertEqual(problem["written_date"], "2024-02-29")

    def test_empty_report_has_a_selectable_current_year(self):
        activity = board.build_activity([], "2026-10-10")
        self.assertEqual(activity["daily_counts"], {})
        self.assertEqual(activity["years"], [2026])

    def test_years_include_history_and_current_year(self):
        activity = board.build_activity([
            {"written_date": "2024-12-31"},
            {"written_date": "2025-01-01"},
        ], "2026-10-10")
        self.assertEqual(activity["years"], [2026, 2025, 2024])
        self.assertEqual(activity["daily_counts"], {"2024-12-31": 1, "2025-01-01": 1})


if __name__ == "__main__":
    unittest.main()
