"""
Google Drive Downloader - Handles downloading files from Google Drive
"""
import requests
import os
from pathlib import Path

class GoogleDriveDownloader:
    def __init__(self, config):
        self.config = config
        self.base_url = "https://drive.google.com/uc?export=download"
        
    def download_version_with_info(self, version, version_info, log_callback, progress_callback=None):
        """Download version from Google Drive using provided version info"""
        if not version_info:
            raise Exception(f"No configuration found for version {version}")
            
        file_id = version_info.get('file_id')
        if not file_id:
            raise Exception(f"No Google Drive file ID found for version {version}")
            
        return self._download_file(file_id, version, log_callback, progress_callback)
        
    def download_version(self, version, log_callback):
        """Download version from Google Drive (legacy method)"""
        # Get file ID from config
        versions_data = self.config.get('versions', {})
        version_info = versions_data.get(version)
        
        if not version_info:
            raise Exception(f"No configuration found for version {version}")
            
        file_id = version_info.get('file_id')
        if not file_id:
            raise Exception(f"No Google Drive file ID found for version {version}")
        
        return self._download_file(file_id, version, log_callback)
    
    def _download_file(self, file_id, version, log_callback, progress_callback=None):
        """Internal method to download file from Google Drive"""
        log_callback(f"Connecting to Google Drive...")
        log_callback(f"File ID: {file_id}")
        
        # Download file
        session = requests.Session()
        
        # Initial request
        response = session.get(self.base_url, params={'id': file_id}, stream=True)
        
        # Handle Google Drive virus scan warning for large files (>100MB)
        # Google returns HTML page with confirmation token
        token = None
        for key, value in response.cookies.items():
            if key.startswith('download_warning'):
                token = value
                break
        
        # Also check for 'confirm' parameter in cookies (newer Google Drive format)
        if not token:
            for key, value in response.cookies.items():
                if 'confirm' in key.lower():
                    token = value
                    break
                
        if token:
            log_callback(f"Large file detected, confirming download...")
            params = {'id': file_id, 'confirm': token}
            response = session.get(self.base_url, params=params, stream=True)
        
        # For very large files, try alternative confirmation methods
        if not token and 'text/html' in response.headers.get('content-type', ''):
            log_callback(f"Large file detected (>100MB), bypassing virus scan warning...")
            
            # Try multiple confirmation methods for large files
            confirmation_attempts = ['t', '1', 'true', 'yes']
            
            for confirm_value in confirmation_attempts:
                params = {'id': file_id, 'confirm': confirm_value, 'export': 'download'}
                test_response = session.get(self.base_url, params=params, stream=True)
                
                # Check if we got the file (not HTML)
                if 'text/html' not in test_response.headers.get('content-type', ''):
                    log_callback(f"✓ Bypassed virus scan warning")
                    response = test_response
                    break
            
            # If still HTML, try the direct download URL format
            if 'text/html' in response.headers.get('content-type', ''):
                log_callback(f"Trying direct download URL...")
                direct_url = f"https://drive.usercontent.google.com/download?id={file_id}&export=download&confirm=t"
                response = session.get(direct_url, stream=True)
        
        # Final check - if still HTML, something is wrong
        content_type = response.headers.get('content-type', '')
        if 'text/html' in content_type and response.status_code == 200:
            # Read a bit of the response to see what it is
            peek = response.content[:500].decode('utf-8', errors='ignore')
            if 'quota' in peek.lower():
                raise Exception(
                    "Google Drive download quota exceeded.\n"
                    "Please try again later."
                )
            else:
                raise Exception(
                    f"Failed to download from Google Drive.\n"
                    f"Response type: {content_type}\n"
                    f"Status code: {response.status_code}\n\n"
                    f"The file might be:\n"
                    f"- Not shared publicly (verify in incognito mode)\n"
                    f"- The file ID is incorrect\n"
                    f"- Google Drive link restrictions\n"
                    f"- Download quota exceeded"
                )
        
        # Get file size
        total_size = int(response.headers.get('content-length', 0))
        
        # Save file
        download_dir = Path.cwd() / "downloads"
        download_dir.mkdir(exist_ok=True)
        
        # Use version-specific filename
        download_path = download_dir / f"LEKMOD_{version}.zip"
        
        # Download with progress
        log_callback(f"Downloading to {download_path}...")
        
        if total_size > 0:
            log_callback(f"File size: {self._format_size(total_size)}")
        
        downloaded = 0
        chunk_size = 32768  # 32KB chunks
        last_update = 0
        update_interval = 512 * 1024  # Update every 512KB (more frequent)
        
        with open(download_path, 'wb') as f:
            for chunk in response.iter_content(chunk_size=chunk_size):
                if chunk:
                    f.write(chunk)
                    downloaded += len(chunk)
                    
                    # Update progress more frequently (every 512KB or at completion)
                    if (downloaded - last_update) >= update_interval or downloaded == total_size:
                        if total_size > 0:
                            progress = (downloaded / total_size) * 100
                            
                            # Update progress bar if callback provided
                            if progress_callback:
                                progress_callback(progress, 
                                    f"Downloading... {self._format_size(downloaded)} / "
                                    f"{self._format_size(total_size)}")
                            
                            log_callback(
                                f"Downloading... {self._format_size(downloaded)} / "
                                f"{self._format_size(total_size)} ({progress:.1f}%)"
                            )
                        else:
                            log_callback(f"Downloading... {self._format_size(downloaded)}")
                        last_update = downloaded
        
        log_callback(f"✓ Downloaded successfully!")
        return str(download_path)
    
    def _format_size(self, bytes_size):
        """Format bytes to human-readable size"""
        for unit in ['B', 'KB', 'MB', 'GB']:
            if bytes_size < 1024.0:
                return f"{bytes_size:.1f} {unit}"
            bytes_size /= 1024.0
        return f"{bytes_size:.1f} TB"
    
    def get_file_info(self, file_id):
        """Get file information from Google Drive (optional, requires API key)"""
        # This would require Google Drive API setup
        # For now, return placeholder
        return {
            'name': 'Lekmod.zip',
            'size': 'Unknown',
            'modified': 'Unknown'
        }

