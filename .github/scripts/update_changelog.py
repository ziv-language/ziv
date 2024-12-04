# Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
# See /LICENSE for license details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#!/usr/bin/env python3

import os
import re
from datetime import datetime
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

def get_latest_release_date() -> Optional[str]:
    """Get the date of the latest release from the changelog."""
    if not Path(CHANGELOG_PATH).exists():
        return None
        
    with open(CHANGELOG_PATH, 'r') as f:
        content = f.read()
        
    dates = re.findall(r'\[x\.x\.x\] - (\d{4}-\d{2}-\d{2})', content)
    return dates[0] if dates else None

def get_commits_since_date(date: Optional[str]) -> List[str]:
    """Get all commits since the given date."""
    if date:
        date_obj = datetime.strptime(date, '%Y-%m-%d')
        git_cmd = ['git', 'log', '--since', date_obj.strftime('%Y-%m-%d'), '--format=%H %s']
    else:
        git_cmd = ['git', 'log', '--format=%H %s']
        
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
    now = datetime.now().strftime('%Y-%m-%d')
    
    if not Path(CHANGELOG_PATH).exists():
        # Create new changelog if it doesn't exist
        with open(CHANGELOG_PATH, 'w') as f:
            f.write('''# CHANGELOG

All notable changes to this project will be documented in this file.  
This project adheres to [Semantic Versioning](https://semver.org/).

## [Unreleased]

''')
    
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
    
    # Create the new content
    new_changes = []
    for category, changes in categorized_commits.items():
        if changes:
            new_changes.append(f"\n### {category}\n")
            for change in changes:
                new_changes.append(f"- {change}\n")
    
    # Insert the new changes after the [Unreleased] section
    if new_changes:
        content = (
            content[:unreleased_idx + 1] +
            new_changes +
            content[unreleased_idx + 1:]
        )
        
        # Write the updated content
        with open(CHANGELOG_PATH, 'w') as f:
            f.writelines(content)

def main():
    latest_release_date = get_latest_release_date()
    commits = get_commits_since_date(latest_release_date)
    categorized = categorize_commits(commits)
    update_changelog(categorized)

if __name__ == "__main__":
    main()
