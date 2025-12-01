"""
Update Checker - Manages version checking from Google Drive
"""
import requests
import json
from datetime import datetime

class UpdateChecker:
    def __init__(self, config):
        self.config = config
        self.version_url = config.get('version_check_url')
        
    def get_available_versions(self):
        """Get list of available versions (fresh from GitHub each time)"""
        versions = {}
        
        # Try to fetch from online JSON first
        if self.version_url:
            try:
                # Add cache-busting parameter to avoid GitHub caching
                import time
                cache_bust = f"?t={int(time.time())}"
                url_with_cache_bust = self.version_url + cache_bust
                
                response = requests.get(url_with_cache_bust, timeout=10)
                if response.status_code == 200:
                    online_versions = response.json()
                    if online_versions:
                        return online_versions
                else:
                    print(f"GitHub returned status code: {response.status_code}")
            except Exception as e:
                print(f"Failed to fetch online versions: {e}")
                # Don't return here - try fallback
        
        # Fallback to local config
        if 'versions' in self.config:
            versions.update(self.config['versions'])
        
        # If still empty, that's an error
        if not versions:
            raise Exception("No versions available from GitHub or local config")
        
        return versions
    
    def compare_versions(self, v1, v2):
        """Compare two version strings (e.g., v35.2 vs v35.1)"""
        def parse_version(v):
            # Remove 'v' prefix and split
            v = v.lower().replace('v', '')
            parts = v.split('.')
            return [int(p) for p in parts if p.isdigit()]
        
        try:
            v1_parts = parse_version(v1)
            v2_parts = parse_version(v2)
            
            # Compare each part
            for i in range(max(len(v1_parts), len(v2_parts))):
                p1 = v1_parts[i] if i < len(v1_parts) else 0
                p2 = v2_parts[i] if i < len(v2_parts) else 0
                
                if p1 > p2:
                    return 1
                elif p1 < p2:
                    return -1
            
            return 0  # Equal
        except:
            return 0
    
    def get_latest_version(self, versions):
        """Get the latest version from a dict of versions"""
        if not versions:
            return None
        
        version_list = list(versions.keys())
        version_list.sort(key=lambda v: self.parse_version_key(v), reverse=True)
        
        return version_list[0] if version_list else None
    
    def parse_version_key(self, version):
        """Parse version string to sortable tuple"""
        try:
            v = version.lower().replace('v', '')
            parts = v.split('.')
            return tuple(int(p) for p in parts if p.isdigit())
        except:
            return (0,)

