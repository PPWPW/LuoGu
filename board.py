#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""洛谷刷题展板，只使用 Python 标准库。

    python board.py                 扫描代码，生成 board.html 并打开浏览器
    python board.py --no-open       只生成网页
    python board.py --offline       使用本地题目信息，不联网
    python board.py --refresh       从洛谷更新题名和难度
    python board.py --summary       只输出统计

统计依据是源码文件，不是洛谷的 AC 记录。同题多份代码只计一道题。
题号优先取文件名，再取最近的题号文件夹，例如 P1048/dfs.cpp。
以 ! 或 ！ 开头的目录视为 AI 代码，不计入统计。
每日题数按同题最早一份源码的修改日期估算（北京时间），每题只计一次。
"""

from __future__ import annotations

import argparse
import http.cookiejar
import json
import os
import re
import sys
import time
import urllib.error
import urllib.request
import webbrowser
from collections import Counter
from datetime import datetime, timedelta, timezone
from html.parser import HTMLParser
from pathlib import Path


CHINA_TIME = timezone(timedelta(hours=8))
SOURCE_EXTENSIONS = {
    ".c", ".cc", ".cpp", ".cxx", ".py", ".java", ".pas", ".rs",
    ".go", ".js", ".ts", ".cs", ".kt", ".swift", ".rb", ".lua",
}
IGNORED_DIRECTORIES = {
    "node_modules", "__pycache__", "build", "dist", "target", "venv", "env",
}
PROBLEM_PATTERN = re.compile(
    r"(?<![A-Z0-9])((?:UVA|SP|P|B|T|U)\d+|CF\d+[A-Z]\d*|AT_[A-Z0-9_]+)",
    re.IGNORECASE,
)
# ID 和名称来自洛谷 /_lfe/config 的 ProblemDifficulty，采用 2026 年新版体系。
DIFFICULTIES = [
    {"id": 1, "name": "入门", "color": "#fe4c61"},
    {"id": 2, "name": "普及−", "color": "#f39c32"},
    {"id": 3, "name": "普及", "color": "#e6b634"},
    {"id": 4, "name": "普及+/提高−", "color": "#52c41a"},
    {"id": 5, "name": "提高", "color": "#18a9b8"},
    {"id": 6, "name": "提高+/省选−", "color": "#3498db"},
    {"id": 7, "name": "省选/NOI−", "color": "#9d65d8"},
    {"id": 8, "name": "NOI/NOI+/CTS", "color": "#34415f"},
    {"id": 0, "name": "暂无评定", "color": "#97a3b1"},
    {"id": -1, "name": "待获取", "color": "#c0c8d2"},
]
DIFFICULTY_BY_ID = {item["id"]: item for item in DIFFICULTIES}

# 以下是 2026-10-05 从洛谷公开题目页面核实的数据，保证现有题目离线可用。
BOOTSTRAP_METADATA = {
    "B3637": {"title": "最长上升子序列", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1000": {"title": "超级玛丽游戏", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P1001": {"title": "A+B Problem", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P1003": {"title": "[NOIP 2011 提高组] 铺地毯", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1008": {"title": "[NOIP 1998 普及组] 三连击", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1012": {"title": "[NOIP 1998 提高组] 拼数", "difficulty": 4, "fetched_at": "2026-10-05"},
    "P1014": {"title": "[NOIP 1999 普及组] Cantor 表", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1025": {"title": "[NOIP 2001 提高组] 数的划分", "difficulty": 3, "fetched_at": "2026-10-05"},
    "P1028": {"title": "[NOIP 2001 普及组] 数的计算", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1035": {"title": "[NOIP 2002 普及组] 级数求和", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P1036": {"title": "[NOIP 2002 普及组] 选数", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1042": {"title": "[NOIP 2003 普及组] 乒乓球", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1046": {"title": "[NOIP 2005 普及组] 陶陶摘苹果", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P1047": {"title": "[NOIP 2005 普及组] 校门外的树", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P1048": {"title": "[NOIP 2005 普及组] 采药", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1051": {"title": "[NOIP 2005 提高组] 谁拿了最多奖学金", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1055": {"title": "[NOIP 2008 普及组] ISBN 号码", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1059": {"title": "[NOIP 2006 普及组] 明明的随机数", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1067": {"title": "[NOIP 2009 普及组] 多项式输出", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1068": {"title": "[NOIP 2009 普及组] 分数线划定", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1085": {"title": "[NOIP 2004 普及组] 不高兴的津津", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P1089": {"title": "[NOIP 2004 提高组] 津津的储蓄计划", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P1111": {"title": "修复公路", "difficulty": 3, "fetched_at": "2026-10-05"},
    "P1177": {"title": "【模板】排序", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1179": {"title": "[NOIP 2010 普及组] 数字统计", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P1192": {"title": "台阶问题", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1307": {"title": "[NOIP 2011 普及组] 数字反转", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P1308": {"title": "[NOIP 2011 普及组] 统计单词数", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1309": {"title": "[NOIP 2011 普及组] 瑞士轮", "difficulty": 3, "fetched_at": "2026-10-05"},
    "P1328": {"title": "[NOIP 2014 提高组] 生活大爆炸版石头剪刀布", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1421": {"title": "小玉买文具", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P1427": {"title": "小鱼的数字游戏", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P1464": {"title": "[PacNW 1999] Function", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1563": {"title": "[NOIP 2016 提高组] 玩具谜题", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P1909": {"title": "[NOIP 2016 普及组] 买铅笔", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P1980": {"title": "[NOIP 2013 普及组] 计数问题", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P2010": {"title": "[NOIP 2016 普及组] 回文日期", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P2141": {"title": "[NOIP 2014 普及组] 珠心算测验", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P2482": {"title": "[SDOI2010] 猪国杀", "difficulty": 7, "fetched_at": "2026-10-05"},
    "P2615": {"title": "[NOIP 2015 提高组] 神奇的幻方", "difficulty": 2, "fetched_at": "2026-10-05"},
    "P3375": {"title": "【模板】KMP", "difficulty": 4, "fetched_at": "2026-10-05"},
    "P3378": {"title": "【模板】堆", "difficulty": 3, "fetched_at": "2026-10-05"},
    "P3952": {"title": "[NOIP 2017 提高组] 时间复杂度", "difficulty": 4, "fetched_at": "2026-10-05"},
    "P4391": {"title": "[BalticOI 2009] Radio Transmission 无线传输", "difficulty": 4, "fetched_at": "2026-10-05"},
    "P4994": {"title": "终于结束的起点", "difficulty": 3, "fetched_at": "2026-10-05"},
    "P5015": {"title": "[NOIP 2018 普及组] 标题统计", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P5380": {"title": "[THUPC 2019] 鸭棋", "difficulty": 6, "fetched_at": "2026-10-05"},
    "P5534": {"title": "【XR-3】等差数列", "difficulty": 1, "fetched_at": "2026-10-05"},
    "P5587": {"title": "打字练习", "difficulty": 3, "fetched_at": "2026-10-05"},
    "P5594": {"title": "【XR-4】模拟赛", "difficulty": 1, "fetched_at": "2026-10-05"},
}


def problem_sort_key(pid: str) -> tuple:
    """自然排序，使 P999 在 P1000 之前。"""
    return tuple(
        (1, int(part)) if part.isdigit() else (0, part.lower())
        for part in re.split(r"(\d+)", pid) if part
    )


def scan_problems(root: Path) -> dict[str, list[dict]]:
    """只扫描个人源码；跳过 AI、隐藏和生成目录、链接及无题号文件。"""
    root = root.resolve()
    if root.name.startswith(("!", "！")):
        return {}
    board_path = Path(__file__).resolve()
    groups: dict[str, list[dict]] = {}
    for directory, folders, filenames in os.walk(root):
        folders[:] = sorted(
            name for name in folders
            if not name.startswith((".", "!", "！"))
            and name.lower() not in IGNORED_DIRECTORIES
            and not (Path(directory) / name).is_symlink()
            and not getattr(Path(directory) / name, "is_junction", lambda: False)()
        )
        for filename in sorted(filenames):
            source = Path(directory) / filename
            if (filename.startswith(".") or source.suffix.lower() not in SOURCE_EXTENSIONS
                    or source.is_symlink() or source.resolve() == board_path):
                continue
            relative = source.relative_to(root)
            match = PROBLEM_PATTERN.search(source.stem)
            if match is None:
                match = next(
                    (candidate for part in reversed(relative.parts[:-1])
                     if (candidate := PROBLEM_PATTERN.search(part)) is not None),
                    None,
                )
            if match is None:
                continue
            try:
                stat = source.stat()
            except OSError:
                continue
            groups.setdefault(match.group(1).upper(), []).append({
                "path": relative.as_posix(), "modified": stat.st_mtime,
            })
    return dict(sorted(groups.items(), key=lambda pair: problem_sort_key(pair[0])))


def valid_metadata(record: object) -> bool:
    return (
        isinstance(record, dict)
        and isinstance(record.get("title"), str)
        and bool(record["title"].strip())
        and type(record.get("difficulty")) is int
        and record["difficulty"] in DIFFICULTY_BY_ID
        and record["difficulty"] >= 0
        and isinstance(record.get("fetched_at"), str)
        and re.fullmatch(r"\d{4}-\d{2}-\d{2}", record["fetched_at"]) is not None
    )


def load_metadata(cache_path: Path) -> dict:
    records = {pid: dict(record) for pid, record in BOOTSTRAP_METADATA.items()}
    try:
        cache = json.loads(cache_path.read_text(encoding="utf-8"))
        if isinstance(cache, dict):
            for pid, record in cache.items():
                if not isinstance(pid, str) or not valid_metadata(record):
                    continue
                if pid not in records or record["fetched_at"] >= records[pid]["fetched_at"]:
                    records[pid] = record
    except (OSError, ValueError):
        pass
    return records


class ContextParser(HTMLParser):
    """读取洛谷新版网页内嵌的 JSON，而不是猜测题目难度。"""

    def __init__(self) -> None:
        super().__init__()
        self.inside = False
        self.chunks: list[str] = []

    def handle_starttag(self, tag: str, attrs: list) -> None:
        if tag == "script" and dict(attrs).get("id") == "lentille-context":
            self.inside = True

    def handle_endtag(self, tag: str) -> None:
        if tag == "script":
            self.inside = False

    def handle_data(self, data: str) -> None:
        if self.inside:
            self.chunks.append(data)


def parse_problem_page(body: str, pid: str) -> dict:
    if body.lstrip().startswith("{"):
        context = json.loads(body)
    else:
        parser = ContextParser()
        parser.feed(body)
        context = json.loads("".join(parser.chunks))
    data = context.get("data", context.get("currentData", {}))
    problem = data.get("problem", {})
    if problem.get("pid", pid).upper() != pid.upper():
        raise ValueError("返回的题号与请求不一致")
    record = {
        "title": problem.get("name") or problem.get("title"),
        "difficulty": problem.get("difficulty"),
        "fetched_at": datetime.now(CHINA_TIME).strftime("%Y-%m-%d"),
    }
    if not valid_metadata(record):
        raise ValueError("网页未返回有效题名或已知难度，请稍后重试")
    return record


def update_metadata(pids: list[str], records: dict, cache_path: Path) -> list[str]:
    # 匿名 Cookie 仅在本次运行的内存中保存，避免洛谷 CDN 的重定向循环。
    opener = urllib.request.build_opener(
        urllib.request.HTTPCookieProcessor(http.cookiejar.CookieJar())
    )
    errors: list[str] = []
    updated = False
    consecutive_network_errors = 0
    for index, pid in enumerate(pids):
        request = urllib.request.Request(
            f"https://www.luogu.com.cn/problem/{pid}",
            headers={"User-Agent": "Mozilla/5.0", "Referer": "https://www.luogu.com.cn/"},
        )
        try:
            with opener.open(request, timeout=10) as response:
                records[pid] = parse_problem_page(response.read().decode("utf-8"), pid)
            updated = True
            consecutive_network_errors = 0
            print(f"[{index + 1}/{len(pids)}] 已更新 {pid}", file=sys.stderr, flush=True)
        except (urllib.error.URLError, OSError, ValueError, KeyError, TypeError) as exc:
            errors.append(f"{pid}: {exc}")
            if isinstance(exc, (urllib.error.URLError, OSError)):
                consecutive_network_errors += 1
            else:
                consecutive_network_errors = 0
            # 离线或限流时及时返回，仍可用内置数据和已有缓存生成展板。
            if consecutive_network_errors >= 3:
                errors.append(f"网络连续失败，其余 {len(pids) - index - 1} 道暂未更新。")
                break
        if index + 1 < len(pids):
            time.sleep(0.25)
    if updated:
        temporary = cache_path.with_suffix(".tmp")
        try:
            temporary.write_text(json.dumps(records, ensure_ascii=False, indent=2), encoding="utf-8")
            temporary.replace(cache_path)
        except OSError as exc:
            errors.append(f"缓存保存失败（本次展板仍包含更新数据）: {exc}")
    return errors


def build_activity(problems: list[dict], today: str) -> dict:
    """按最早一份源码的修改日期归档，每个题号只贡献一次。"""
    daily_counts = Counter(problem["written_date"] for problem in problems)
    years = {int(today[:4])}
    years.update(int(day[:4]) for day in daily_counts)
    return {
        "today": today,
        "daily_counts": dict(sorted(daily_counts.items())),
        "years": sorted(years, reverse=True),
        "note": "按最早一份源码的修改日期估算，每题只计一次（北京时间）；已排除 ! / ！ 开头的目录。",
    }


def build_report(root: Path, groups: dict, records: dict, errors: list[str] | None = None) -> dict:
    now = datetime.now(CHINA_TIME)
    problems = []
    for pid, files in groups.items():
        record = records.get(pid, {})
        difficulty = record.get("difficulty", -1)
        if difficulty not in DIFFICULTY_BY_ID:
            difficulty = -1
        modified = max(file["modified"] for file in files)
        first_modified = min(file["modified"] for file in files)
        problems.append({
            "id": pid, "title": record.get("title", "题目信息待获取"),
            "difficulty": difficulty, "files": files, "modified": modified,
            "modified_date": datetime.fromtimestamp(modified, CHINA_TIME).strftime("%Y-%m-%d %H:%M"),
            "written_date": datetime.fromtimestamp(first_modified, CHINA_TIME).strftime("%Y-%m-%d"),
            "fetched_at": record.get("fetched_at", ""),
            "url": f"https://www.luogu.com.cn/problem/{pid}",
        })
    counts = Counter(problem["difficulty"] for problem in problems)
    levels = [{**level, "count": counts[level["id"]]} for level in DIFFICULTIES]
    fetched_dates = sorted({problem["fetched_at"] for problem in problems if problem["fetched_at"]})
    return {
        "root": str(root.resolve()), "folder": root.resolve().name,
        "generated_at": now.strftime("%Y-%m-%d %H:%M"),
        "metadata_dates": fetched_dates,
        "total": len(problems), "source_count": sum(len(problem["files"]) for problem in problems),
        "known_count": len(problems) - counts[-1],
        "highest": max((problem["difficulty"] for problem in problems), default=-1),
        "covered_levels": sum(counts[level] > 0 for level in range(1, 9)),
        "levels": levels, "problems": problems, "errors": errors or [],
        "activity": build_activity(problems, now.strftime("%Y-%m-%d")),
    }


PAGE_TEMPLATE = r'''<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="color-scheme" content="light dark">
<title>我的洛谷刷题展板</title>
<style>
:root{color-scheme:light;--bg:#f5f6f8;--surface:#fff;--ink:#202d30;--muted:#788489;--line:#e9edef;--accent:#14866b;--soft:#edf7f3;--sidebar:#182d2a;--shadow:0 5px 24px #23393004}
*{box-sizing:border-box}html{scroll-behavior:smooth}body{margin:0;background:var(--bg);color:var(--ink);font:14px/1.6 "Segoe UI","Microsoft YaHei","PingFang SC",sans-serif}
button,input,select{font:inherit}button,a,input,select{outline-offset:4px}button{cursor:pointer}a{color:inherit;text-decoration:none}button{color:inherit}svg{vertical-align:middle}
.sidebar{position:fixed;inset:0 auto 0 0;width:224px;padding:36px 24px;background:var(--sidebar);color:#edf7f2;display:flex;flex-direction:column;z-index:2}
.brand{display:flex;align-items:center;gap:12px}.logo{display:grid;place-items:center;width:42px;height:42px;border-radius:12px;background:#d5f3e4;color:#254c40;font:800 24px Georgia,serif}.brand strong{font-size:17px;letter-spacing:.4px}.brand small{display:block;color:#8fa8a0;font-size:10px;letter-spacing:2px}
.nav{display:grid;gap:9px;margin-top:48px}.nav a{padding:12px 16px;border-radius:9px;color:#a9beb5;display:flex;align-items:center;gap:12px}.nav a:first-child{background:#ffffff0d;color:#e2f5eb}.nav a:hover{background:#ffffff14;color:white}
.sidebar-bottom{margin-top:auto;border-top:1px solid #ffffff14;padding-top:20px;font-size:12px;color:#92aaa0}.sidebar-bottom strong{color:#dbece3;display:block;margin-top:8px;font-weight:500;overflow-wrap:anywhere}
main{max-width:1560px;margin-left:224px;padding:28px 42px 32px}.topbar{display:flex;align-items:center;justify-content:space-between;gap:12px;font-size:12px;color:var(--muted)}.topbar strong{color:var(--ink);font-weight:500}.top-actions{display:flex;align-items:center;gap:12px}.status{display:flex;align-items:center;gap:7px}.status:before{content:"";width:6px;height:6px;background:var(--accent);border-radius:50%}.icon-button{display:grid;place-items:center;width:32px;height:32px;border:1px solid var(--line);background:var(--surface);border-radius:8px;color:var(--muted)}
.hero{margin:37px 0 26px;display:flex;justify-content:space-between;align-items:center;gap:20px}.eyebrow{font-size:11px;font-weight:600;letter-spacing:2.5px;color:var(--accent)}h1{margin:7px 0 8px;font-size:32px;letter-spacing:-1px;font-weight:700}.hero p{margin:0;color:var(--muted);font-size:13px}.milestone{background:var(--soft);padding:15px 20px;border-radius:12px;width:230px;flex-shrink:0}.milestone-label{display:flex;justify-content:space-between;font-size:12px;color:var(--accent)}.milestone-label strong{font-size:15px}.track{height:5px;background:#16866b16;border-radius:8px;overflow:hidden;margin:10px 0 7px}.track span{display:block;height:100%;border-radius:8px;background:var(--accent)}.milestone small{color:var(--muted);font-size:11px}
.stats{display:grid;grid-template-columns:repeat(4,minmax(0,1fr));gap:16px}.stat{padding:21px 24px;border:1px solid var(--line);border-radius:13px;background:var(--surface);box-shadow:var(--shadow)}.stat-label{display:flex;align-items:center;gap:8px;color:var(--muted);font-size:12px}.stat-number{font-size:37px;font-weight:650;line-height:1.25;margin:13px 0 6px;letter-spacing:-1px}.stat-number small{font-size:12px;font-weight:400;letter-spacing:0;color:var(--muted);margin-left:6px}.stat-number.difficulty-value{font-size:21px;line-height:46px;letter-spacing:0;white-space:nowrap}.stat-note{font-size:11px;color:var(--muted)}.stat:first-child .stat-number{color:var(--accent)}
.charts{display:grid;grid-template-columns:minmax(0,1.5fr) minmax(0,1fr);gap:20px;margin-top:22px}.panel{background:var(--surface);border:1px solid var(--line);border-radius:13px;padding:24px;box-shadow:var(--shadow)}.panel-header{display:flex;justify-content:space-between;align-items:center;gap:12px}.panel h2{font-size:15px;margin:0;font-weight:650}.hint{font-size:11px;color:var(--muted)}.bars{display:grid;gap:13px;margin-top:22px}.bar-row{display:grid;grid-template-columns:116px minmax(0,1fr) 30px 42px;align-items:center;gap:12px;border:0;background:none;padding:0;text-align:left;width:100%;font-size:11px}.bar-row:hover .bar-label{color:var(--accent)}.bar-label{white-space:nowrap;display:flex;gap:8px;align-items:center}.dot{display:inline-block;width:7px;height:7px;border-radius:50%;flex-shrink:0}.bar-track{height:7px;background:var(--bg);border-radius:5px;overflow:hidden}.bar-fill{display:block;height:100%;border-radius:5px}.bar-count{font-weight:600;text-align:right;font-variant-numeric:tabular-nums}.bar-percent{color:var(--muted);text-align:right;font-size:10px}.zero{opacity:.5}.mix-content{display:flex;align-items:center;justify-content:center;gap:28px;min-height:262px}.donut{width:166px;height:166px;border-radius:50%;position:relative;flex-shrink:0;background:var(--line)}.donut:after{content:"";position:absolute;inset:17px;border-radius:50%;background:var(--surface)}.donut-center{position:absolute;inset:0;display:flex;justify-content:center;align-items:center;flex-direction:column;z-index:1}.donut-center strong{font-size:35px;font-weight:650;line-height:1.25;letter-spacing:-1px}.donut-center span{font-size:11px;color:var(--muted)}.legend{display:grid;gap:13px;min-width:112px}.legend-row{display:flex;align-items:center;gap:8px;font-size:11px}.legend-row strong{margin-left:auto;padding-left:10px;font-size:12px}.mix-note{border-top:1px solid var(--line);padding-top:13px;color:var(--muted);font-size:11px;text-align:center}
.archive{margin-top:23px;padding-bottom:0}.archive .panel-header{align-items:flex-start}.subtitle{color:var(--muted);font-size:11px;margin:5px 0 0}.secondary-button{display:inline-flex;align-items:center;gap:7px;border:1px solid var(--line);background:var(--surface);padding:7px 12px;border-radius:7px;font-size:11px;white-space:nowrap}.secondary-button:hover{border-color:var(--accent);color:var(--accent)}.tools{display:flex;justify-content:space-between;align-items:center;gap:16px;margin:21px 0 16px}.search-box{display:flex;align-items:center;gap:9px;max-width:350px;flex:1;border:1px solid var(--line);border-radius:8px;padding:8px 11px;color:var(--muted);background:var(--bg)}.search-box:focus-within{border-color:var(--accent)}.search-box input{border:0;background:none;outline:none;width:100%;font-size:12px;color:var(--ink);min-width:0}.search-box input::placeholder{color:var(--muted)}select{border:1px solid var(--line);border-radius:7px;background:var(--surface);color:var(--muted);padding:8px;font-size:11px}.filters{display:flex;gap:7px;flex-wrap:wrap;margin-bottom:18px}.chip{border:1px solid var(--line);background:var(--surface);border-radius:6px;padding:5px 9px;font-size:10px;display:flex;align-items:center;gap:6px}.chip.active{border-color:var(--accent);background:var(--soft);color:var(--accent)}.chip .chip-count{opacity:.55}
.table-wrap{overflow-x:auto}table{width:100%;border-collapse:collapse;text-align:left;font-size:12px}thead{background:var(--bg);color:var(--muted);font-size:10px}th{font-weight:500;padding:11px 12px;white-space:nowrap}td{border-bottom:1px solid var(--line);padding:14px 12px}tr:last-child td{border-bottom:0}tbody tr:hover{background:var(--soft)}.pid{font-family:Consolas,monospace;font-weight:600;color:var(--accent);white-space:nowrap}.title-cell{min-width:220px;max-width:450px}.problem-link:hover{text-decoration:underline;text-underline-offset:4px}.badge{display:inline-block;padding:3px 8px;border-radius:5px;font-size:10px;white-space:nowrap;background:var(--badge-bg);color:var(--badge-color)}.source-cell{font-size:11px;min-width:125px;color:var(--muted)}details summary{cursor:pointer;white-space:nowrap}details ul{padding-left:14px;margin:8px 0 0;font-family:Consolas,monospace;font-size:10px;overflow-wrap:anywhere;max-width:260px}.date-cell{color:var(--muted);font:10px Consolas,monospace;white-space:nowrap}.empty{padding:50px;text-align:center;color:var(--muted)}.pagination{display:flex;justify-content:space-between;align-items:center;gap:12px;border-top:1px solid var(--line);padding:15px 0;color:var(--muted);font-size:11px}.pager{display:flex;gap:8px;align-items:center}.pager button{padding:5px 10px;border:1px solid var(--line);border-radius:5px;background:var(--surface);font-size:11px}.pager button:disabled{opacity:.35;cursor:default}.pager select{padding:4px}
.activity{margin-top:22px;--heat-0:#edf0ee;--heat-1:#a4dfaa;--heat-2:#63bc78;--heat-3:#2e9254;--heat-4:#176c3b}.activity .panel-header{align-items:flex-start}.activity-year{display:flex;align-items:center;gap:8px;color:var(--muted);font-size:11px;white-space:nowrap}.activity-year select{min-width:80px;padding:5px 8px}.heatmap-scroll{overflow-x:auto;padding:18px 0 8px;scrollbar-width:thin}.heatmap-calendar{--heat-size:13px;--heat-gap:4px;display:grid;grid-template-columns:23px auto;gap:7px 9px;width:max-content}.heatmap-months{grid-column:2;display:grid;grid-template-columns:repeat(var(--weeks),var(--heat-size));gap:var(--heat-gap);font-size:10px;color:var(--muted);line-height:16px}.heatmap-months span{white-space:nowrap}.heatmap-weekdays{display:grid;grid-template-rows:repeat(7,var(--heat-size));gap:var(--heat-gap);font-size:9px;color:var(--muted);line-height:var(--heat-size)}.heatmap-days{display:grid;grid-auto-flow:column;grid-template-rows:repeat(7,var(--heat-size));grid-template-columns:repeat(var(--weeks),var(--heat-size));gap:var(--heat-gap)}.heat-day,.heat-key{width:var(--heat-size,13px);height:var(--heat-size,13px);border:1px solid #15202108;border-radius:3px;background:var(--heat-0);padding:0}.heat-day[data-count="1"],.heat-key[data-count="1"]{background:var(--heat-1)}.heat-day[data-count="2"],.heat-key[data-count="2"]{background:var(--heat-2)}.heat-day[data-count="3"],.heat-key[data-count="3"]{background:var(--heat-3)}.heat-day[data-count="4"],.heat-key[data-count="4"]{background:var(--heat-4)}.heat-day:hover,.heat-day:focus-visible{outline:2px solid var(--accent);outline-offset:2px}.heat-day.outside{visibility:hidden}.heat-day.future{opacity:.3;cursor:default}.heatmap-bottom{display:flex;align-items:center;justify-content:space-between;gap:12px;margin-top:7px;color:var(--muted);font-size:11px}.heatmap-detail{min-height:18px;margin:0}.heatmap-legend{display:flex;align-items:center;gap:5px;white-space:nowrap;font-size:10px}.heatmap-empty{margin:9px 0 0;color:var(--muted);font-size:12px}.activity-note{margin-top:13px;padding-top:12px;border-top:1px solid var(--line);font-size:10px;color:var(--muted)}
footer{display:flex;justify-content:space-between;gap:16px;margin-top:23px;font-size:10px;color:var(--muted)}footer a:hover{color:var(--accent)}.notice{border:1px solid #e4ba6255;background:#e4ba6210;border-radius:9px;padding:12px 16px;font-size:12px;margin-top:18px;color:var(--muted)}.notice code{color:var(--ink)}.footer-instruction{margin-top:6px}code{font-family:Consolas,monospace}
body.dark{color-scheme:dark;--bg:#152021;--surface:#1e2b2d;--ink:#e0e9e6;--muted:#95a5a1;--line:#2b3b3c;--accent:#78d8b1;--soft:#233d36;--sidebar:#101b1b;--shadow:none}.dark .milestone .track{background:#78d8b126}.dark .badge{background:var(--badge-dark-bg);color:var(--badge-dark-color)}.dark .activity{--heat-0:#29393a;--heat-1:#285d3c;--heat-2:#338d4c;--heat-3:#54b567;--heat-4:#8be18a}.dark .heat-day,.dark .heat-key{border-color:#ffffff08}
@media(max-width:650px){.activity{margin-top:16px}.heatmap-bottom{align-items:flex-start;flex-direction:column;gap:8px}.activity-note{line-height:1.8}.activity-year{gap:5px}.activity-year select{min-width:70px}}
@media(min-width:1450px){main{padding-left:58px;padding-right:58px}}@media(max-width:1180px){.sidebar{width:190px;padding:30px 18px}main{margin-left:190px;padding:24px}.stats{gap:12px}.stat{padding:18px}.charts{grid-template-columns:minmax(0,1.2fr) minmax(0,1fr);gap:16px}.mix-content{gap:18px;flex-wrap:wrap;padding:20px 0}.donut{width:150px;height:150px}.legend{min-width:112px}.panel{padding:20px}.bar-row{gap:8px;grid-template-columns:111px minmax(0,1fr) 25px 37px}}
@media(max-width:950px){.charts{grid-template-columns:1fr}.mix-content{min-height:200px;flex-wrap:nowrap;gap:50px}.legend{grid-template-columns:1fr 1fr;gap:15px 22px}.stats{grid-template-columns:repeat(2,minmax(0,1fr))}.milestone{width:200px}h1{font-size:28px}}
@media(max-width:650px){.sidebar{position:static;width:auto;padding:16px 20px;flex-direction:row;align-items:center;justify-content:space-between}.brand strong{font-size:14px}.brand small{font-size:8px}.logo{height:34px;width:34px;font-size:21px}.nav{display:flex;margin:0;gap:4px}.nav a{padding:8px;font-size:11px;gap:5px}.nav svg{display:none}.sidebar-bottom{display:none}main{margin:0;padding:20px 16px}.topbar{font-size:10px}.top-actions{gap:6px}.hero{margin:27px 0 20px;align-items:flex-start;flex-direction:column;gap:18px}.milestone{width:100%;padding:12px 16px}.stat{padding:16px}.stat-number{font-size:32px}.stat-number.difficulty-value{font-size:17px;line-height:40px}.stats{gap:10px}.charts{margin-top:16px;gap:16px}.panel{padding:18px}.mix-content{gap:24px}.legend{grid-template-columns:1fr;gap:10px}.donut{width:138px;height:138px}.donut:after{inset:14px}.bar-row{grid-template-columns:108px minmax(0,1fr) 22px 37px;gap:7px}.archive{margin-top:16px}.tools{align-items:stretch;gap:8px}.search-box{max-width:none}.tools select{max-width:125px}.pagination{flex-wrap:wrap}.pager{margin-left:auto}footer{flex-direction:column;gap:8px}.hint{font-size:10px}.status{display:none}}
@media print{body{background:white;color:#202d30}.sidebar,.top-actions,.milestone,.tools,.filters,.pagination,.secondary-button,.notice{display:none}main{margin:0;padding:10px;max-width:none}.hero{margin:15px 0}.stats{grid-template-columns:repeat(4,1fr)}.charts{grid-template-columns:1.4fr 1fr}.panel,.stat{break-inside:avoid;box-shadow:none}.archive{break-inside:auto}tr{break-inside:avoid}*{-webkit-print-color-adjust:exact;print-color-adjust:exact}.table-wrap{overflow:visible}body.dark{--bg:#f5f6f8;--surface:#fff;--ink:#202d30;--muted:#788489;--line:#e9edef;--accent:#14866b;--soft:#edf7f3}.mix-content{gap:18px;flex-wrap:nowrap}.legend{grid-template-columns:1fr}.donut{width:140px;height:140px}}
</style>
</head>
<body>
<aside class="sidebar">
  <div class="brand"><div class="logo">L</div><div><strong>洛谷 · 题迹</strong><small>MY CODING JOURNEY</small></div></div>
  <nav class="nav" aria-label="展板导航"><a href="#overview"><svg width="17" height="17" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5"><rect x="3" y="3" width="7" height="7" rx="1.5"/><rect x="14" y="3" width="7" height="7" rx="1.5"/><rect x="3" y="14" width="7" height="7" rx="1.5"/><rect x="14" y="14" width="7" height="7" rx="1.5"/></svg>数据概览</a><a href="#archive"><svg width="17" height="17" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5"><path d="M4 4h16v16H4zM8 8h8M8 12h8M8 16h5"/></svg>题目档案</a></nav>
  <div class="sidebar-bottom">每一次思考，都留下痕迹。<strong id="folder-name"></strong><span>本地代码 · 自动统计</span></div>
</aside>
<main id="overview">
  <div class="topbar"><span>个人练习 <span style="margin:0 8px;opacity:.4">/</span> <strong>刷题展板</strong></span><div class="top-actions"><span class="status">本地展板</span><button class="icon-button" id="theme-toggle" aria-label="切换深浅色主题" title="切换主题">◐</button><button class="icon-button" id="print-button" aria-label="打印展板" title="打印展板"><svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5"><path d="M7 8V3h10v5M7 17H3V9h18v8h-4M7 14h10v7H7z"/></svg></button></div></div>
  <div class="hero"><div><div class="eyebrow">A LITTLE PROGRESS, EVERY DAY</div><h1>我的刷题展板<span style="color:var(--accent)">.</span></h1><p>把写过的题目，变成看得见的积累。</p></div><div class="milestone"><div class="milestone-label"><span>下一个十题里程碑</span><strong id="next-goal"></strong></div><div class="track"><span id="goal-progress"></span></div><small id="goal-note"></small></div></div>
  <section class="stats" aria-label="刷题统计"><article class="stat"><div class="stat-label">累计写过</div><div class="stat-number"><span id="total-count">0</span><small>道题</small></div><div class="stat-note">按题号去重，每道题只记一次</div></article><article class="stat"><div class="stat-label">源码积累</div><div class="stat-number"><span id="source-count">0</span><small>份代码</small></div><div class="stat-note">记录同一道题的不同实现</div></article><article class="stat"><div class="stat-label">最高题目难度</div><div class="stat-number difficulty-value" id="highest-level">—</div><div class="stat-note" id="highest-note"></div></article><article class="stat"><div class="stat-label">已覆盖难度</div><div class="stat-number"><span id="covered-count">0</span><small>/ 8 档</small></div><div class="stat-note" id="known-note"></div></article></section>
  <div id="notice"></div>
  <section class="panel activity" id="activity" aria-label="每日刷题热力图"><div class="panel-header"><div><h2>每日题迹</h2><p class="subtitle" id="activity-summary"></p></div><label class="activity-year">年份 <select id="activity-year" aria-label="选择热力图年份"></select></label></div><div class="heatmap-scroll" id="heatmap-scroll"><div class="heatmap-calendar" id="heatmap-calendar"><div class="heatmap-months" id="heatmap-months"></div><div class="heatmap-weekdays" aria-hidden="true"><span>日</span><span>一</span><span>二</span><span>三</span><span>四</span><span>五</span><span>六</span></div><div class="heatmap-days" id="heatmap-days" aria-label="每日写题数量"></div></div></div><div class="heatmap-bottom"><p class="heatmap-detail" id="activity-detail" aria-live="polite">悬停或选中方格，查看当天写题数量。</p><div class="heatmap-legend" aria-label="颜色越深，当天写题越多"><span>少</span><i class="heat-key" data-count="0" title="0 道题"></i><i class="heat-key" data-count="1" title="1 道题"></i><i class="heat-key" data-count="2" title="2–3 道题"></i><i class="heat-key" data-count="3" title="4–6 道题"></i><i class="heat-key" data-count="4" title="7 道题及以上"></i><span>多</span></div></div><p class="heatmap-empty" id="activity-empty" hidden>这一年还没有写题记录，从下一道题开始点亮日历。</p><div class="activity-note" id="activity-note"></div></section>
  <section class="charts" aria-label="难度分布"><article class="panel"><div class="panel-header"><h2>难度分布</h2><span class="hint">点击难度查看题目</span></div><div class="bars" id="difficulty-bars"></div></article><article class="panel"><div class="panel-header"><h2>题目构成</h2><span class="hint">按洛谷难度统计</span></div><div class="mix-content"><div class="donut" id="donut"><div class="donut-center"><strong id="donut-total">0</strong><span>道题目</span></div></div><div class="legend" id="legend"></div></div><div class="mix-note" id="mix-note"></div></article></section>
  <section class="panel archive" id="archive"><div class="panel-header"><div><h2>题目档案</h2><p class="subtitle">每道题的难度与源码，一目了然。</p></div><button class="secondary-button" id="export-button"><span aria-hidden="true">↓</span> 导出清单</button></div><div class="tools"><label class="search-box"><svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8" aria-hidden="true"><circle cx="10.5" cy="10.5" r="6.5"/><path d="m16 16 5 5"/></svg><input id="search" type="search" placeholder="搜索题号、题名或文件名…" aria-label="搜索题目"></label><select id="sort" aria-label="题目排序"><option value="id">按题号排序</option><option value="difficulty-desc">难度从高到低</option><option value="difficulty-asc">难度从低到高</option><option value="modified">最近修改优先</option></select></div><div class="filters" id="filters" aria-label="难度筛选"></div><div class="table-wrap"><table><thead><tr><th>题号</th><th>题目</th><th>难度</th><th>源码</th><th>最近修改</th></tr></thead><tbody id="problem-rows"></tbody></table></div><div class="pagination"><span id="result-count" aria-live="polite"></span><div class="pager"><select id="page-size" aria-label="每页题目数量"><option value="10">10 条 / 页</option><option value="20">20 条 / 页</option><option value="all">全部题目</option></select><button id="previous" aria-label="上一页">←</button><span id="page-label"></span><button id="next" aria-label="下一页">→</button></div></div></section>
  <footer><div>统计依据：本地源码文件，按题号去重；不代表洛谷 AC 记录。<div class="footer-instruction">新增代码后运行 <code>python board.py</code> 更新展板 · <code>--refresh</code> 同步洛谷难度</div></div><div><span id="generated-at"></span><br><a href="https://help.luogu.com.cn/manual/luogu/problem/difficulty" target="_blank" rel="noopener noreferrer">洛谷 2026 年难度体系 ↗</a> · <span id="metadata-date"></span></div></footer>
  <noscript><p class="notice">请启用浏览器 JavaScript 查看统计，也可以运行 python board.py --summary 查看终端统计。</p></noscript>
</main>
<script id="board-data" type="application/json">__BOARD_DATA__</script>
<script>
"use strict";
const data = JSON.parse(document.getElementById("board-data").textContent);
const $ = id => document.getElementById(id);
const levels = new Map(data.levels.map(level => [level.id, level]));
const escape = value => String(value).replace(/[&<>"']/g, char => ({"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#39;"}[char]));
const state = {difficulty: null, query: "", sort: "id", page: 1, pageSize: 10};
const naturalOrder = new Intl.Collator("en", {numeric: true, sensitivity: "base"});
$("folder-name").textContent = data.folder;
$("folder-name").title = data.root;
$("total-count").textContent = data.total;
$("source-count").textContent = data.source_count;
$("covered-count").textContent = data.covered_levels;
$("donut-total").textContent = data.total;
$("known-note").textContent = `已获取 ${data.known_count} / ${data.total} 道题的难度`;
const highest = levels.get(data.highest);
$("highest-level").textContent = data.highest > 0 ? highest.name : "暂无已评定题目";
$("highest-level").style.color = data.highest > 0 ? highest.color : "var(--muted)";
$("highest-note").textContent = data.highest > 0 ? `本地有 ${highest.count} 道该难度题目` : "写下第一份代码，开始积累";
const goal = (Math.floor(data.total / 10) + 1) * 10;
$("next-goal").textContent = `${goal} 题`;
$("goal-progress").style.width = `${data.total / goal * 100}%`;
$("goal-note").textContent = `已积累 ${data.total} 道，再写 ${goal - data.total} 道到达下一站。`;
$("generated-at").textContent = `展板更新于 ${data.generated_at}（北京时间）`;
const dates = data.metadata_dates;
$("metadata-date").textContent = dates.length ? `难度核实于 ${dates[0]}${dates.length > 1 ? " ~ " + dates[dates.length - 1] : ""}` : "难度待获取";
if (data.known_count < data.total || data.errors.length) {
  const notice = document.createElement("div"); notice.className = "notice";
  notice.textContent = `${data.total - data.known_count} 道题的难度待获取。${data.errors.length ? "部分洛谷数据未能更新，已有题目保留本地信息。" : ""} 联网后运行 python board.py --refresh 重试。`;
  $("notice").appendChild(notice);
}
const activity = data.activity;
const activityToday = activity.today;
const activityCurrentYear = Number(activityToday.slice(0, 4));
$("activity-year").innerHTML = activity.years.map(year => `<option value="${year}">${year} 年</option>`).join("");
$("activity-year").value = String(activityCurrentYear);
$("activity-note").textContent = activity.note;
const dateKey = date => date.toISOString().slice(0, 10);
function renderActivity() {
  const year = Number($("activity-year").value);
  const first = new Date(Date.UTC(year, 0, 1));
  const last = new Date(Date.UTC(year, 11, 31));
  const dayMs = 86400000;
  const offset = first.getUTCDay();
  const dayCount = Math.round((last - first) / dayMs) + 1;
  const weeks = Math.ceil((offset + dayCount) / 7);
  $("heatmap-calendar").style.setProperty("--weeks", weeks);
  $("heatmap-months").innerHTML = Array.from({length: 12}, (_, month) => {
    const monthOffset = Math.round((Date.UTC(year, month, 1) - first.getTime()) / dayMs);
    return `<span style="grid-column:${Math.floor((offset + monthOffset) / 7) + 1}">${month + 1}月</span>`;
  }).join("");
  let total = 0, activeDays = 0;
  const cells = [];
  for (let index = 0; index < weeks * 7; index++) {
    const day = index - offset;
    if (day < 0 || day >= dayCount) {
      cells.push('<span class="heat-day outside" aria-hidden="true"></span>');
      continue;
    }
    const key = dateKey(new Date(first.getTime() + day * dayMs));
    const future = key > activityToday;
    const count = future ? 0 : Number(activity.daily_counts[key] || 0);
    if (count > 0) {total += count; activeDays++;}
    const level = count >= 7 ? 4 : count >= 4 ? 3 : count >= 2 ? 2 : count > 0 ? 1 : 0;
    const description = future ? `${key}：尚未到来` : `${key}：写了 ${count} 道题`;
    cells.push(`<button type="button" class="heat-day${future ? " future" : ""}" data-date="${key}" data-count="${level}" title="${description}" aria-label="${description}" tabindex="${day === 0 ? "0" : "-1"}"${future ? " disabled" : ""}></button>`);
  }
  $("heatmap-days").innerHTML = cells.join("");
  $("activity-summary").textContent = `${year} 年写了 ${total} 道题 · 活跃 ${activeDays} 天`;
  $("activity-empty").hidden = total > 0;
  $("activity-detail").textContent = "悬停或选中方格，查看当天写题数量。";
}
function showActivityDay(button, focus = false) {
  if (!button || !button.dataset.date) return;
  $("activity-detail").textContent = button.title;
  if (focus) {
    $("heatmap-days").querySelectorAll("button").forEach(day => day.tabIndex = day === button ? 0 : -1);
    button.focus({preventScroll: true});
    button.scrollIntoView({block: "nearest", inline: "nearest"});
  }
}
$("activity-year").addEventListener("change", renderActivity);
$("heatmap-days").addEventListener("mouseover", event => showActivityDay(event.target.closest("[data-date]")));
$("heatmap-days").addEventListener("focusin", event => showActivityDay(event.target.closest("[data-date]")));
$("heatmap-days").addEventListener("click", event => showActivityDay(event.target.closest("button:not(:disabled)"), true));
$("heatmap-days").addEventListener("keydown", event => {
  const moves = {ArrowLeft: -7, ArrowRight: 7, ArrowUp: -1, ArrowDown: 1};
  if (!(event.key in moves) && event.key !== "Home" && event.key !== "End") return;
  const days = [...$("heatmap-days").querySelectorAll("button:not(:disabled)")];
  const current = days.indexOf(event.target);
  if (current < 0) return;
  event.preventDefault();
  const next = event.key === "Home" ? 0 : event.key === "End" ? days.length - 1 : Math.max(0, Math.min(days.length - 1, current + moves[event.key]));
  showActivityDay(days[next], true);
});
renderActivity();
const chartLevels = data.levels.filter(level => level.id > 0 || level.count > 0);
const maximum = Math.max(1, ...chartLevels.map(level => level.count));
$("difficulty-bars").innerHTML = chartLevels.map(level => {
  const percentage = data.total ? level.count / data.total * 100 : 0;
  return `<button class="bar-row ${level.count ? "" : "zero"}" data-level="${level.id}" aria-label="查看${escape(level.name)}题目：${level.count}道"><span class="bar-label"><i class="dot" style="background:${level.color}"></i>${escape(level.name)}</span><span class="bar-track"><span class="bar-fill" style="width:${level.count / maximum * 100}%;background:${level.color}"></span></span><span class="bar-count">${level.count}</span><span class="bar-percent">${percentage.toFixed(0)}%</span></button>`;
}).join("");
const presentLevels = chartLevels.filter(level => level.count > 0);
let start = 0;
const gradient = presentLevels.map(level => {
  const end = start + level.count / data.total * 100;
  const part = `${level.color} ${start}% ${end}%`; start = end; return part;
});
if (gradient.length) $("donut").style.background = `conic-gradient(${gradient.join(",")})`;
$("donut").setAttribute("role", "img");
$("donut").setAttribute("aria-label", presentLevels.map(level => `${level.name}${level.count}道`).join("，") || "暂无题目");
$("legend").innerHTML = presentLevels.map(level => `<div class="legend-row"><i class="dot" style="background:${level.color}"></i><span>${escape(level.name)}</span><strong>${level.count}</strong></div>`).join("");
const mostCommon = presentLevels.reduce((best, level) => !best || level.count > best.count ? level : best, null);
$("mix-note").textContent = mostCommon ? `${mostCommon.name}题目最多，占全部题目的 ${(mostCommon.count / data.total * 100).toFixed(0)}%` : "从第一道题开始，记录你的算法之旅。";
$("filters").innerHTML = [{id: "all", name: "全部", count: data.total}, ...chartLevels].map(level => `<button class="chip" data-filter="${level.id}">${level.color ? `<i class="dot" style="background:${level.color}"></i>` : ""}${escape(level.name)}<span class="chip-count">${level.count}</span></button>`).join("");

function filteredProblems() {
  return data.problems.filter(problem =>
    (state.difficulty === null || problem.difficulty === state.difficulty) &&
    `${problem.id} ${problem.title} ${problem.files.map(file => file.path).join(" ")}`.toLowerCase().includes(state.query)
  ).sort((a, b) => {
    if (state.sort === "modified") return b.modified - a.modified || naturalOrder.compare(a.id, b.id);
    if (state.sort.startsWith("difficulty")) {
      const delta = a.difficulty - b.difficulty;
      if (delta) return state.sort.endsWith("desc") ? -delta : delta;
    }
    return naturalOrder.compare(a.id, b.id);
  });
}
function renderTable(printAll = false) {
  const problems = filteredProblems();
  const size = state.pageSize === "all" ? Math.max(1, problems.length) : state.pageSize;
  const pages = Math.max(1, Math.ceil(problems.length / size));
  state.page = Math.min(state.page, pages);
  const offset = (state.page - 1) * size;
  const visible = printAll ? problems : problems.slice(offset, offset + size);
  $("problem-rows").innerHTML = visible.map(problem => {
    const level = levels.get(problem.difficulty);
    return `<tr><td><a class="pid problem-link" href="${escape(problem.url)}" target="_blank" rel="noopener noreferrer">${escape(problem.id)}</a></td><td class="title-cell"><a class="problem-link" href="${escape(problem.url)}" target="_blank" rel="noopener noreferrer">${escape(problem.title)}</a></td><td><span class="badge" style="--badge-bg:${level.color}15;--badge-color:${level.color};--badge-dark-bg:${level.color}28;--badge-dark-color:${level.color}">${escape(level.name)}</span></td><td class="source-cell"><details><summary>${problem.files.length} 份代码</summary><ul>${problem.files.map(file => `<li>${escape(file.path)}</li>`).join("")}</ul></details></td><td class="date-cell">${escape(problem.modified_date)}</td></tr>`;
  }).join("") || '<tr><td class="empty" colspan="5">没有找到匹配的题目，试试其他题号或难度。</td></tr>';
  $("result-count").textContent = problems.length ? `显示 ${offset + 1}–${Math.min(offset + size, problems.length)} 条，共 ${problems.length} 道题` : "共 0 道题";
  $("page-label").textContent = `${state.page} / ${pages}`;
  $("previous").disabled = state.page <= 1;
  $("next").disabled = state.page >= pages;
  document.querySelectorAll("[data-filter]").forEach(button => {
    const active = button.dataset.filter === (state.difficulty === null ? "all" : String(state.difficulty));
    button.classList.toggle("active", active);
    button.setAttribute("aria-pressed", String(active));
  });
}
$("filters").addEventListener("click", event => {
  const button = event.target.closest("[data-filter]"); if (!button) return;
  state.difficulty = button.dataset.filter === "all" ? null : Number(button.dataset.filter);
  state.page = 1; renderTable();
});
$("difficulty-bars").addEventListener("click", event => {
  const button = event.target.closest("[data-level]"); if (!button) return;
  state.difficulty = Number(button.dataset.level); state.page = 1; renderTable();
  $("archive").scrollIntoView({behavior: "smooth", block: "start"});
});
$("search").addEventListener("input", event => {state.query = event.target.value.trim().toLowerCase(); state.page = 1; renderTable();});
$("sort").addEventListener("change", event => {state.sort = event.target.value; state.page = 1; renderTable();});
$("page-size").addEventListener("change", event => {state.pageSize = event.target.value === "all" ? "all" : Number(event.target.value); state.page = 1; renderTable();});
$("previous").addEventListener("click", () => {state.page--; renderTable();});
$("next").addEventListener("click", () => {state.page++; renderTable();});
$("theme-toggle").addEventListener("click", () => {
  document.body.classList.toggle("dark");
  try {localStorage.setItem("luogu-board-theme", document.body.classList.contains("dark") ? "dark" : "light");} catch (_) {}
});
try {if (localStorage.getItem("luogu-board-theme") === "dark") document.body.classList.add("dark");} catch (_) {}
$("export-button").addEventListener("click", () => {
  const rows = [["题号", "题目", "难度", "源码文件", "最近修改", "难度获取日期", "洛谷链接"], ...filteredProblems().map(problem => [problem.id, problem.title, levels.get(problem.difficulty).name, problem.files.map(file => file.path).join("; "), problem.modified_date, problem.fetched_at, problem.url])];
  const csv = rows.map(row => row.map(value => '"' + String(value).replace(/"/g, '""') + '"').join(",")).join("\r\n");
  const url = URL.createObjectURL(new Blob(["\uFEFF", csv], {type: "text/csv;charset=utf-8"}));
  const link = document.createElement("a"); link.href = url; link.download = "洛谷刷题清单.csv";
  document.body.appendChild(link); link.click(); link.remove(); setTimeout(() => URL.revokeObjectURL(url), 1000);
});
$("print-button").addEventListener("click", () => window.print());
window.addEventListener("beforeprint", () => renderTable(true));
window.addEventListener("afterprint", () => renderTable());
renderTable();
</script>
</body>
</html>
'''


def render_html(report: dict) -> str:
    # 防止文件名或远程题名中的 </script> 提前结束内嵌 JSON。
    payload = json.dumps(report, ensure_ascii=False).replace("<", "\\u003c")
    return PAGE_TEMPLATE.replace("__BOARD_DATA__", payload)


def main() -> int:
    parser = argparse.ArgumentParser(description="扫描本地题目代码，生成洛谷刷题展板（无需安装依赖）。")
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parent, help="题目文件夹，默认 board.py 所在目录")
    parser.add_argument("--output", type=Path, help="网页输出路径，默认题目文件夹下的 board.html")
    parser.add_argument("--no-open", action="store_true", help="生成展板后不自动打开浏览器")
    parser.add_argument("--offline", action="store_true", help="只使用内置数据和缓存，不联网")
    parser.add_argument("--refresh", action="store_true", help="重新读取洛谷题名和难度，失败时保留已有数据")
    parser.add_argument("--summary", action="store_true", help="只输出终端统计，不生成网页")
    args = parser.parse_args()
    if args.refresh and args.offline:
        parser.error("--refresh 和 --offline 不能同时使用")
    root = args.root.resolve()
    if not root.is_dir():
        parser.error(f"题目文件夹不存在: {root}")
    groups = scan_problems(root)
    cache_path = root / ".luogu-board-cache.json"
    records = load_metadata(cache_path)
    errors = []
    if not args.offline:
        pending = [pid for pid in groups if args.refresh or pid not in records]
        if pending:
            errors = update_metadata(pending, records, cache_path)
    report = build_report(root, groups, records, errors)
    print(f"已写 {report['total']} 道题，共 {report['source_count']} 份源码（同题去重，已排除 ! / ！ 开头的目录）。")
    for level in report["levels"]:
        if level["id"] > 0 or level["count"]:
            print(f"  {level['name']:<14} {level['count']:>3} 道")
    for error in errors:
        print(f"数据更新提示: {error}", file=sys.stderr)
    if args.summary:
        return 0
    output = args.output.resolve() if args.output else root / "board.html"
    try:
        output.write_text(render_html(report), encoding="utf-8")
    except OSError as exc:
        print(f"展板保存失败: {exc}", file=sys.stderr)
        return 1
    print(f"展板已生成: {output}")
    if not args.no_open:
        try:
            opened = webbrowser.open(output.as_uri())
        except (OSError, webbrowser.Error):
            opened = False
        if not opened:
            print("浏览器未能自动打开，请手动打开上面的 HTML 文件。")
    return 0


if __name__ == "__main__":
    for stream in (sys.stdout, sys.stderr):
        if hasattr(stream, "reconfigure"):
            stream.reconfigure(encoding="utf-8")
    raise SystemExit(main())
