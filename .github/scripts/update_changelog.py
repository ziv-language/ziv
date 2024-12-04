# Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
# See /LICENSE for license details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#!/usr/bin/env python3

import os
import re
from datetime import datetime, timedelta
from typing import Dict, List, Optional
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
    "perf": "Changed"
}

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

def get_current_month_commits() -> List[str]:
    """Get commits from the last month until now."""
    today = datetime.now()
    first_day_current_month = today.replace(day=1)
    last_day_previous_month = first_day_current_month - timedelta(days=1)
    first_day_previous_month = last_day_previous_month.replace(day=1)
    
    date_since = first_day_previous_month.strftime('%Y-%m-%d')
    date_until = first_day_current_month.strftime('%Y-%m-%d')
    
    git_cmd = ['git', 'log', f'--since={date_since}', f'--until={date_until}', '--format=%H %s']
    result = subprocess.run(git_cmd, capture_output=True, text=True)
    return result.stdout.strip().split('\n')

def categorize_commits(commits: List[str]) -> Dict[str, List[str]]:
    """Categorize commits by their type."""
    categorized: Dict[str, List[str]] = {
        "Added": [],
        "Fixed": [],
        "Changed": [],
        "Security": [],
        "Deprecated": [],
        "Removed": []
    }
    
    for commit in commits:
        if not commit.strip():
            continue
            
        hash_msg = commit.split(' ', 1)
        if len(hash_msg) != 2:
            continue
            
        _, msg = hash_msg
        parsed = parse_conventional_commit(msg)
        
        if parsed:
            commit_type = COMMIT_TYPES.get(parsed['type'], 'Changed')
            description = parsed['description']
            if parsed['scope']:
                description = f"{parsed['scope']}: {description}"
            categorized[commit_type].append(description)
    
    return categorized

def update_changelog(categorized_commits: Dict[str, List[str]]) -> None:
    """Update the CHANGELOG.md file with new changes."""
    current_month = datetime.now().strftime('%B %Y')
    
    if not Path(CHANGELOG_PATH).exists():
        print(f"Error: {CHANGELOG_PATH} does not exist")
        return
    
    with open(CHANGELOG_PATH, 'r') as f:
        content = f.readlines()
    
    # Find the [Unreleased] section
    unreleased_idx = -1
    for i, line in enumerate(content):
        if '[Unreleased]' in line:
            unreleased_idx = i
            break
    
    if unreleased_idx == -1:
        print("Could not find [Unreleased] section")
        return
    
    # Create the new month section
    new_month_content = [f"\n### {current_month}\n\n"]
    
    # Add categorized changes
    for category in ["Added", "Fixed", "Changed", "Security", "Deprecated", "Removed"]:
        changes = categorized_commits.get(category, [])
        if changes:
            new_month_content.append(f"#### {category}\n")
            for change in changes:
                new_month_content.append(f"- {change}\n")
            new_month_content.append("\n")
    
    # Insert the new month section after [Unreleased]
    content = (
        content[:unreleased_idx + 1] +
        new_month_content +
        content[unreleased_idx + 1:]
    )
    
    # Write the updated content
    with open(CHANGELOG_PATH, 'w') as f:
        f.writelines(content)

def main():
    commits = get_current_month_commits()
    categorized = categorize_commits(commits)
    update_changelog(categorized)

if __name__ == "__main__":
    main()
