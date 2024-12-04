# Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
# See /LICENSE for license details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#!/usr/bin/env python3

import os
import re
from datetime import datetime, timedelta
from typing import Dict, List, Optional, NamedTuple
import subprocess
from pathlib import Path

CHANGELOG_PATH = "CHANGELOG.md"
COMMIT_TYPES = {
    "feat": "Added",
    "fix": "Fixed",
    "docs": "Changed",
    "chore": "Changed",
    "refactor": "Changed",
    "style": "Changed",
    "test": "Changed",
    "perf": "Changed",
    "ci": "Changed"
}

class CommitInfo(NamedTuple):
    hash: str
    description: str
    date: str

def get_commit_date(hash: str) -> Optional[str]:
    """Get the date for a commit."""
    try:
        date_cmd = ['git', 'show', '-s', '--format=%cd', '--date=format:%d-%m-%Y', hash]
        date_result = subprocess.run(date_cmd, capture_output=True, text=True)
        return date_result.stdout.strip()
    except subprocess.SubprocessError:
        return None

def parse_conventional_commit(commit_message: str) -> Optional[Dict[str, str]]:
    """Parse a conventional commit message."""
    pattern = r'^(?P<type>\w+)(?:\((?P<scope>[^)]+)\))?: (?P<description>.+)$'
    match = re.match(pattern, commit_message)
    
    if not match:
        return None
        
    return {
        'type': match.group('type'),
        'scope': match.group('scope'),
        'description': match.group('description')
    }

def get_current_month_commits() -> List[tuple[str, str]]:
    """Get commits from the last month until now."""
    today = datetime.now()
    first_day_current_month = today.replace(day=1)
    last_day_previous_month = first_day_current_month - timedelta(days=1)
    first_day_previous_month = last_day_previous_month.replace(day=1)
    
    date_since = first_day_previous_month.strftime('%Y-%m-%d')
    date_until = first_day_current_month.strftime('%Y-%m-%d')
    
    git_cmd = ['git', 'log', f'--since={date_since}', f'--until={date_until}', '--format=%H %s']
    result = subprocess.run(git_cmd, capture_output=True, text=True)
    commits = result.stdout.strip().split('\n')
    return [(line.split(' ', 1)[0], line.split(' ', 1)[1]) 
            for line in commits if line.strip()]

def get_previous_month() -> str:
    """Get the previous month name and year."""
    today = datetime.now()
    first_day_current_month = today.replace(day=1)
    last_day_previous_month = first_day_current_month - timedelta(days=1)
    return last_day_previous_month.strftime('%B %Y')

def categorize_commits(commits: List[tuple[str, str]]) -> Dict[str, List[CommitInfo]]:
    """Categorize commits by their type."""
    categorized: Dict[str, List[CommitInfo]] = {
        "Added": [],
        "Fixed": [],
        "Changed": [],
        "Security": [],
        "Deprecated": [],
        "Removed": []
    }
    
    for commit_hash, msg in commits:
        parsed = parse_conventional_commit(msg)
        if parsed:
            commit_type = COMMIT_TYPES.get(parsed['type'], 'Changed')
            description = parsed['description']
            if parsed['scope']:
                description = f"{parsed['scope']}: {description}"
                
            date = get_commit_date(commit_hash)
            if date:
                commit_info = CommitInfo(
                    hash=commit_hash,
                    description=description,
                    date=date
                )
                categorized[commit_type].append(commit_info)
    
    # Sort by date but don't remove duplicates
    return {k: sorted(v, key=lambda x: x.date) for k, v in categorized.items() if v}

def update_changelog(categorized_commits: Dict[str, List[CommitInfo]]) -> None:
    """Update the CHANGELOG.md file with new changes."""
    previous_month = get_previous_month()
    
    if not Path(CHANGELOG_PATH).exists():
        print(f"Error: {CHANGELOG_PATH} does not exist")
        return
    
    with open(CHANGELOG_PATH, 'r') as f:
        content = f.read()

    # Split content into sections
    unreleased_pattern = r'## \[Unreleased\]\n'
    match = re.search(unreleased_pattern, content)
    
    if not match:
        print("Could not find [Unreleased] section")
        return
    
    unreleased_start = match.end()
    
    # Find the next major section
    next_section_match = re.search(r'\n---\n', content[unreleased_start:])
    if next_section_match:
        next_section_start = unreleased_start + next_section_match.start()
    else:
        next_section_start = len(content)

    # Check if the previous month section already exists
    month_pattern = f'### {previous_month}\n'
    if month_pattern in content[unreleased_start:next_section_start]:
        print(f"Section for {previous_month} already exists")
        return
    
    # Create the new month section
    new_month_content = [f"\n### {previous_month}\n\n"]
    
    # Add categorized changes
    for category in ["Added", "Fixed", "Changed", "Security", "Deprecated", "Removed"]:
        changes = categorized_commits.get(category, [])
        if changes:
            new_month_content.append(f"#### {category}\n")
            for commit_info in changes:
                new_month_content.append(f"- {commit_info.description} [{commit_info.date}]\n")
            new_month_content.append("\n")
    
    # Combine the content
    updated_content = (
        content[:unreleased_start] +
        ''.join(new_month_content) +
        content[unreleased_start:]
    )
    
    # Write the updated content
    with open(CHANGELOG_PATH, 'w') as f:
        f.write(updated_content)

def main():
    commits = get_current_month_commits()
    categorized = categorize_commits(commits)
    update_changelog(categorized)

if __name__ == "__main__":
    main()
